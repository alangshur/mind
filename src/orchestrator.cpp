#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::EngineOrchestrator() {}
EngineOrchestrator::~EngineOrchestrator() {
    delete this->elo_store;
    delete this->contribution_store;
}

void EngineOrchestrator::execute() {
    // TODO: Implement
}

void EngineOrchestrator::launch_node() {
    // TODO: Implement
}

void EngineOrchestrator::wait_node_shutdown() {
    unique_lock<mutex> lk(shutdown_mutex);
    shutdown_cv.wait(lk, []{ return shutdown_flag; });
}

void EngineOrchestrator::shutdown_node() {
    // TODO: Implement
}

void EngineOrchestrator::build_core() {
    this->elo_store = new EngineEloStore();
    this->contribution_store = new EngineContributionStore(*(this->elo_store));

    // TODO: Build rest of core
}

void EngineOrchestrator::build_exec() {

    // TODO: Build rest of exec
}

void EngineOrchestrator::build_mpi() {

    // TODO: Build mpi
}

int main(int argc, const char* argv[]) {
    return 0;
}