#include <utility>
#include <iostream>
#include "core/executor.hpp"
using namespace std;

EngineExecutor::EngineExecutor(EngineIngestor& ingestor, EngineEloStore& elo_store, 
    EngineContributionStore& contribution_store, Logger& logger) : ingestor(ingestor), 
    contribution_store(contribution_store), ternary_shutdown_sem(0), 
    shutdown_flag(false), logger(logger) {}

void EngineExecutor::run_contribution_pipeline() {
    cid contribution_id;
    while (true) {

        // fetch and add new contribution
        this->ingestor.new_queue_sem.wait();
        if (this->shutdown_flag) break;
        contribution_id = this->ingestor.new_queue.load()->front();
        try { this->contribution_store.add_contribution(contribution_id); }
        catch (exception& e) { this->logger.log_error("EngineExecutor", e.what()); }
        this->logger.log_message("EngineExecutor", "Successfully added contribution with ID " 
            + to_string(contribution_id) + ".");
        this->ingestor.new_queue.load()->pop();
    }

    // signal shutdown
    ternary_shutdown_sem.post();
}

void EngineExecutor::run_update_pipeline() {
    pair<cid, cid> contribution_ids;
    pair<elo, elo> contribution_elos;

    // run pipeline
    while (true) {

        // fetch new update
        this->ingestor.update_queue_sem.wait();
        if (this->shutdown_flag) break;
        contribution_ids = this->ingestor.update_queue.load()->front();
        this->ingestor.update_queue.load()->pop();

        // calculate ELO scores
        contribution_elos = EngineScorer::update_scores(
            this->contribution_store.fetch_contribution_elo(contribution_ids.first),
            this->contribution_store.fetch_contribution_elo(contribution_ids.second)
        );

        // update contributions
        this->contribution_store.update_contribution(contribution_ids.first, 
            contribution_elos.first);
        this->contribution_store.update_contribution(contribution_ids.second, 
            contribution_elos.second);
        // this->logger.log_message("EngineExecutor", "Successfully added contribution with ID " 
        //     + to_string(contribution_id) + ".");
    }

    // signal shutdown
    ternary_shutdown_sem.post();
}

void EngineExecutor::shutdown_pipelines() {

    // initiate shutdown
    this->shutdown_flag = true;
    this->ingestor.new_queue_sem.post();
    this->ingestor.update_queue_sem.post();

    // wait for pipelines 
    ternary_shutdown_sem.wait();
    ternary_shutdown_sem.wait();
}