#include <iostream>
#include "orchestrator.hpp"
using namespace std;

EngineOrchestrator::~EngineOrchestrator() {

    // free portal pointers
    delete this->ingestion_portal;
    delete this->match_portal;

    // free exec pointers
    delete this->ingestion_exec;
    delete this->match_exec;

    // free core pointers
    delete this->elo_store;
    delete this->contribution_store;
}

void EngineOrchestrator::execute() {

    // execute orchestrator pipeline
    try {
        this->launch_process();
        this->wait_process_shutdown();
        this->shutdown_process();
    }
    catch (exception& e) {
        this->logger.log_error("EngineOrchestrator", "Error executing main orchestrator "
            "pipeline: " + string(e.what()) + ".");
    }

    // exit process
    exit(0);
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
}

void EngineOrchestrator::build_exec() {

    // build ingestion executor
    this->ingestion_exec = new EngineIngestionExecutor(*(this->contribution_store));
    exec_threads.push_back(thread([&](EngineIngestionExecutor* exec) 
        { exec->run(); }, this->ingestion_exec));

    // build match executor
    this->match_exec = new EngineMatchExecutor(*(this->contribution_store));
    exec_threads.push_back(thread([&](EngineMatchExecutor* exec)
        { exec->run(); }, this->match_exec));
}

void EngineOrchestrator::build_portal() {

    // build ingestion portal
    this->ingestion_portal = new EngineIngestionPortal(*(this->ingestion_exec), INGESTION_PORT);
    portal_threads.push_back(thread([&](EngineIngestionPortal* portal) 
        { portal->run(); }, this->ingestion_portal));

    // build match portal
    this->match_portal = new EngineMatchPortal(*(this->match_exec), MATCH_PORT);
    portal_threads.push_back(thread([&](EngineMatchPortal* portal) 
        { portal->run(); }, this->match_portal));
}

void EngineOrchestrator::shutdown_portal() {

    // shutdown portals
    this->ingestion_portal->shutdown();
    this->match_portal->shutdown();

    // join portals
    for (size_t i = 0; i < this->portal_threads.size(); i++) {
        this->portal_threads[i].join();
    }
}

void EngineOrchestrator::shutdown_exec() {
    
    // shutdown executors
    this->ingestion_exec->shutdown();
    this->match_exec->shutdown();

    // join executors
    for (size_t i = 0; i < this->exec_threads.size(); i++) {
        this->exec_threads[i].join();
    }
}

#include "util/iqr-scorer.hpp"

int main(int argc, const char* argv[]) {

    IQRScorer scorer;
    scorer.add_sample(1);
    scorer.add_sample(1);
    scorer.add_sample(2);
    scorer.add_sample(3);
    scorer.add_sample(1);
    scorer.add_sample(4);
    scorer.remove_sample(4);
    scorer.remove_sample(1);
    scorer.remove_sample(2);
    scorer.remove_sample(1);
    scorer.remove_sample(3);
    scorer.remove_sample(1);

    scorer.print_quartile_set();

    // cout << endl << "Q1: " << scorer.get_q1() << endl << flush;
    // cout << "Q2: " << scorer.get_median() << endl << flush;
    // cout << "Q3: " << scorer.get_q3() << endl << endl << flush;
    // cout << "IQR: " << scorer.get_iqr() << endl << flush;
    // cout << "Mean: " << scorer.get_mean() << endl << flush;

    // EngineOrchestrator orchestrator;
    // orchestrator.execute();
    return 0;
}