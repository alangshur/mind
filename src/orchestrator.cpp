#include <iostream>
#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::EngineOrchestrator() {}
EngineOrchestrator::~EngineOrchestrator() {

    // free portal pointers

    // free exec pointers

    // free core pointers
    delete this->elo_store;
    delete this->contribution_store;
}

void EngineOrchestrator::execute() {
    // TODO: Implement
}

void EngineOrchestrator::launch_process() {
    // TODO: Implement
}

void EngineOrchestrator::wait_process_shutdown() {
    unique_lock<mutex> lk(shutdown_mutex);
    shutdown_cv.wait(lk, []{ return shutdown_flag; });
}

void EngineOrchestrator::shutdown_process() {
    // TODO: Implement
}

void EngineOrchestrator::build_core() {

    // build infrastructure core
    this->elo_store = new EngineEloStore();
    this->contribution_store = new EngineContributionStore(*(this->elo_store));

    // TODO: Build rest of core
}

void EngineOrchestrator::build_exec() {

    // build ingestion executor
    this->ingestion_exec = new EngineIngestionExecutor(*(this->contribution_store));
    exec_threads.push_back(thread([&](EngineIngestionExecutor* exec) 
        { exec->run(); }, this->ingestion_exec));

    // TODO: Build rest of exec
}

void EngineOrchestrator::build_portal() {

    // TODO: Build portal
}

#include "util/tcp-server.hpp"
#include "util/tcp-client.hpp"

struct msg_t {
    int id;
    char body[32];
};

int main(int argc, const char* argv[]) {
    
    struct msg_t send_data = {0, "stop"};
    TCPClient<struct msg_t> client;
    client.send_connection("127.0.0.1", 5000);
    client.write_packet(send_data);
    client.close_connection();

    struct msg_t message;
    TCPServer<struct msg_t> server(5000);
    do {
        server.accept_connection();
        message = server.read_packet();
        cout << "Packet received!" << endl;
        cout << "Message ID: " << message.id << endl;
        cout << "Message Body: " << message.body << endl << endl;
        server.close_connection();
    }
    while (string(message.body) != "stop");

    return 0;
}