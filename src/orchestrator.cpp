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
    this->launch_process();
    this->wait_process_shutdown();
    this->shutdown_process();
}

void EngineOrchestrator::launch_process() {
    this->build_core();
    this->build_exec();
    this->build_portal();
}

void EngineOrchestrator::wait_process_shutdown() {
    unique_lock<mutex> lk(global_shutdown_mutex);
    global_shutdown_cv.wait(lk, []{ return global_shutdown_flag; });
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

int main(int argc, const char* argv[]) {
    

    return 0;
}