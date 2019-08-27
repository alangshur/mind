#include "exec/ingestion-executor.hpp"
using namespace std;

EngineIngestionExecutor::EngineIngestionExecutor(EngineContributionStore& 
    contribution_store) : new_queue(new queue<cid>), 
    update_queue(new queue<tuple<cid, elo, bool>>), new_queue_sem(0), 
    update_queue_sem(0), contribution_store(contribution_store), 
    ternary_shutdown_sem(0), shutdown_flag(false) {}

EngineIngestionExecutor::~EngineIngestionExecutor() {
    delete this->new_queue.load();
    delete this->update_queue.load();
}

void EngineIngestionExecutor::run_contribution_pipeline() {
    cid contribution_id;
    
    try {
        while (true) {

            // fetch new contribution
            this->new_queue_sem.wait();
            if (this->shutdown_flag) break;
            contribution_id = this->new_queue.load()->front();

            // add new contribution
            try { this->contribution_store.add_contribution(contribution_id); }
            catch (exception& e) { this->logger.log_error("EngineIngestionExecutor", e.what()); }
            this->logger.log_message("EngineIngestionExecutor", "Successfully added "
                "contribution with ID " + to_string(contribution_id) + ".");
            this->new_queue.load()->pop();
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineIngestionExecutor", "Fatal error in new contribution " 
            "pipeline: " + string(e.what()) + ".");
        this->report_fatal_error();
    }

    // notify shutdown
    this->ternary_shutdown_sem.post();
}

void EngineIngestionExecutor::run_update_pipeline() {
    tuple<cid, elo, bool> update;
    elo updated_elo;

    try {
        while (true) {

            // fetch new update
            this->update_queue_sem.wait();
            if (this->shutdown_flag) break;
            update = this->update_queue.load()->front();
            this->update_queue.load()->pop();

            // calculate updated ELO score
            updated_elo = this->scorer.calculate_rating(
                this->contribution_store.fetch_contribution_elo(get<0>(update)),
                get<1>(update),
                get<2>(update)
            );

            // update contributions
            this->contribution_store.update_contribution(get<0>(update), updated_elo);
            this->logger.log_message("EngineIngestionExecutor", "Successfully updated "
                "contribution with ID " + to_string(get<0>(update)) + " to ELO " 
                + to_string(updated_elo) + " .");
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineIngestionExecutor", "Fatal error in update "
            "pipeline: " + string(e.what()) + ".");
        this->report_fatal_error();
    }

    // notify shutdown
    this->ternary_shutdown_sem.post();
}

void EngineIngestionExecutor::shutdown() {
    this->shutdown_flag = true;
    this->new_queue_sem.post();
    this->update_queue_sem.post();
    this->ternary_shutdown_sem.wait();
    this->ternary_shutdown_sem.wait();
}