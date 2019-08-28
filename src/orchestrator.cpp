#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::EngineOrchestrator() {}
EngineOrchestrator::~EngineOrchestrator() {

    // free mpi pointers

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
        { exec->run_contribution_pipeline(); }, this->ingestion_exec));
    exec_threads.push_back(thread([&](EngineIngestionExecutor* exec) 
        { exec->run_update_pipeline(); }, this->ingestion_exec));

    // TODO: Build rest of exec
}

void EngineOrchestrator::build_mpi() {

    // TODO: Build mpi
}

int main(int argc, const char* argv[]) {
    return 0;
}