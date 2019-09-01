#include <iostream>
#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::~EngineOrchestrator() {

    // free portal pointers

    // free exec pointers
    delete this->ingestion_exec;

    // free core pointers
    delete this->elo_store;
    delete this->contribution_store;
}

void EngineOrchestrator::execute() {
    try {
        this->launch_process();
        this->wait_process_shutdown();
        this->shutdown_process();
    }
    catch (exception& e) {
        this->logger.log_error("EngineOrchestrator", "Error executing main orchestrator "
            "pipeline: " + string(e.what()) + ".");
    }
}

void EngineOrchestrator::launch_process() {
    this->logger.log_message("EngineOrchestrator", "Launching process.");
    this->build_core();
    this->build_exec();
    this->build_portal();
}

void EngineOrchestrator::wait_process_shutdown() {
    this->logger.log_message("EngineOrchestrator", "Waiting for process shutdown.");  
    global_shutdown_sem.wait();
}

void EngineOrchestrator::shutdown_process() {
    this->logger.log_message("EngineOrchestrator", "Shutting down process.");
    this->shutdown_portal();
    this->shutdown_exec();
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

    // build ingestion portal
    this->ingestion_portal = new EngineIngestionPortal(*(this->ingestion_exec), INGESTION_PORT);
    portal_threads.push_back(thread([&](EngineIngestionPortal* portal) 
        { portal->run(); }, this->ingestion_portal));

    // TODO: Build rest of portal
}

void EngineOrchestrator::shutdown_portal() {

    // shutdown portals
    this->ingestion_portal->shutdown();

    // join portals
    for (size_t i = 0; i < this->portal_threads.size(); i++) {
        this->portal_threads[i].join();
    }
}

void EngineOrchestrator::shutdown_exec() {
    
    // shutdown executors
    this->ingestion_exec->shutdown();

    // join executors
    for (size_t i = 0; i < this->exec_threads.size(); i++) {
        this->exec_threads[i].join();
    }
}

#include "util/tcp-client.hpp"

int main(int argc, const char* argv[]) {

    TCPClient<ingestion_packet_t> client;

    // ingestion_packet_t packet;
    // packet.request.type = Contribution;
    // packet.request.data.contribution = {123};
    
    // client.send_connection("127.0.0.1", INGESTION_PORT);
    // client.write_packet(packet);
    // ingestion_packet_t packet_r;
    // client.read_packet(packet_r);
    // cout << "Response: " << packet_r.response << endl;
    // client.close_connection();

    ingestion_packet_t packet;
    packet.request.type = Update;
    packet.request.data.update = {123, 900.0, true};
    
    client.send_connection("127.0.0.1", INGESTION_PORT);
    client.write_packet(packet);
    ingestion_packet_t packet_r;
    client.read_packet(packet_r);
    cout << "Response: " << packet_r.response << endl;
    client.close_connection();

    // EngineOrchestrator orchestrator;
    // orchestrator.execute();
    return 0;
}