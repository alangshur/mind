#include <stdlib.h>
#include <chrono>
#include <iostream>
#include "orchestrator.hpp"
#include "manager/ingestion.hpp"
using namespace std;

EngineIngestor::EngineIngestor(int read_fd, int write_fd) : 
    new_queue(new queue<pair<cid, cid>>), update_queue(new queue<pair<cid, cid>>), 
    new_queue_sem(EffSemaphore(0)), update_queue_sem(EffSemaphore(0)), 
    shutdown_flag(false), input_fd(read_fd), self_input_fd(write_fd) {}

EngineIngestor::~EngineIngestor() { 
    close(this->input_fd); 
    close(this->self_input_fd);
}

void EngineIngestor::run_ingestion() {
    pair<cid, cid> result;
    while (!this->shutdown_flag) {
        result = this->read_fd_packet();
        if (result.first == 0) break;

        // update queues
        if (result.second == 0) {
            this->new_queue.load()->push(result);
            this->new_queue_sem.post();
            cout << "New contribution with ID" << result.first << endl << flush;
        }
        else {
            this->update_queue.load()->push(result);
            this->update_queue_sem.post();
            cout << "Contribution " << result.first << " beat " << result.second << endl << flush;
        }
    }
}

void EngineIngestor::shutdown_ingestor() {
    this->shutdown_flag = true;
    write(this->self_input_fd, SHUTDOWN_PACKET, INGEST_PACKET_BYTES);
}

pair<cid, cid> EngineIngestor::read_fd_packet() {
    cid first = 0, second = 0;
    char buf[INGEST_PACKET_BYTES];
    char* end_ptr;

    // read packet
    read(this->input_fd, buf, INGEST_PACKET_BYTES);
    first = (cid) strtol(buf, &end_ptr, 10);
    second = (cid) strtol(end_ptr, NULL, 10);
    return pair<cid, cid>(first, second);
}