#include <mutex>
#include "portal/ingestion-portal.hpp"
using namespace std;

EngineIngestionPortal::EngineIngestionPortal(EngineIngestionExecutor& executor,
    uint16_t port) : executor(executor), server(port) {}
EngineIngestionPortal::~EngineIngestionPortal() {}

void EngineIngestionPortal::run() {
    try {
        while (true) {

            // read new packet
            this->server.accept_connection();
            if (this->shutdown_flag) break;
            ingestion_packet_t ingestion_req;
            this->server.read_packet(ingestion_req);

            // enqueue packet
            unique_lock<mutex> lk(this->executor.ingestion_queue_mutex);
            this->executor.ingestion_queue.push(ingestion_req.request);
            lk.unlock();

            // signal executor
            this->executor.ingestion_queue_sem.post();

            // write response
            ingestion_packet_t ingestion_res;
            ingestion_res.response = ACK;
            this->server.write_packet(ingestion_res);
            this->server.close_connection();
        }
    }
    catch(exception& e) {
        this->server.force_close_connection();
        if (!this->shutdown_in_progress()) {
            this->logger.log_error("EngineIngestionPortal", "Fatal error: " 
                + string(e.what()) + ".");
            this->report_fatal_error();
        }
    }

    this->notify_shutdown();
}

void EngineIngestionPortal::shutdown() {
    
    // trigger shutdown
    this->shutdown_flag = true;
    this->server.close_acceptor();

    // wait for shutdown
    this->wait_shutdown();
    this->logger.log_message("EngineIngestionPortal", "Successfully shutdown "
        "ingestion portal.");
}