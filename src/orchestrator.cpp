#include <iostream>
#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::EngineOrchestrator() {}
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
    unique_lock<mutex> lk(global_shutdown_mutex);
    global_shutdown_cv.wait(lk, []{ return global_shutdown_flag; });
}

void EngineOrchestrator::shutdown_process() {
    this->logger.log_message("EngineOrchestrator", "Shutting down process.");
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

    // build ingestion portal
    this->ingestion_portal = new EngineIngestionPortal(*(this->ingestion_exec), INGESTION_PORT);
    portal_threads.push_back(thread([&](EngineIngestionPortal* portal) 
        { portal->run(); }, this->ingestion_portal));

    // TODO: Build rest of portal
}

int main(int argc, const char* argv[]) {
    EngineOrchestrator orchestrator;
    orchestrator.execute();
    return 0;
}