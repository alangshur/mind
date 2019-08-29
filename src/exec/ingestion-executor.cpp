#include "exec/ingestion-executor.hpp"
using namespace std;

EngineIngestionExecutor::EngineIngestionExecutor(EngineContributionStore& 
    contribution_store) : contribution_store(contribution_store), 
    shutdown_flag(false) {}

void EngineIngestionExecutor::run() {    
    try {
        while (true) {

            // fetch ingestion
            this->ingestion_queue_sem.wait();
            if (this->shutdown_flag) break;

            // handle contribution ingestion
            unique_lock<mutex> lk(this->ingestion_queue_mutex);
            if (this->ingestion_queue.front().type == Contribution) {
                cid contribution = this->ingestion_queue.front().data.contribution;
                lk.unlock();

                // ingest result
                this->handle_contribution(contribution);
                this->logger.log_message("EngineIngestionExecutor", "Successfully added "
                    "contribution with ID " + to_string(contribution) + ".");
            }

            // handle update ingestion
            else if (this->ingestion_queue.front().type == Update) {
                tuple<cid, elo, bool>& update = this->ingestion_queue.front().data.update;
                lk.unlock();

                // ingest result
                elo updated_elo = this->handle_update(update);
                this->logger.log_message("EngineIngestionExecutor", "Successfully updated "
                    "contribution with ID " + to_string(get<0>(update)) + " to ELO " 
                    + to_string(updated_elo) + " .");
            } 

            // ensure re-lock 
            else lk.unlock();
            this->ingestion_queue.pop();
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineIngestionExecutor", "Fatal error: " + string(e.what()) + ".");
        this->report_fatal_error();
    }

    // notify shutdown
    this->binary_shutdown_sem.post();
}

void EngineIngestionExecutor::handle_contribution(cid contribution) {

    // add new contribution
    this->contribution_store.add_contribution(contribution);
}

elo EngineIngestionExecutor::handle_update(tuple<cid, elo, bool>& update) {

    // calculate new ELO rating
    elo updated_elo = this->scorer.calculate_rating(
        this->contribution_store.fetch_contribution_elo(get<0>(update)),
        get<1>(update),
        get<2>(update)
    );

    // update contribution
    this->contribution_store.update_contribution(get<0>(update), updated_elo);
    return updated_elo;
}

void EngineIngestionExecutor::shutdown() {
    this->shutdown_flag = true;
    this->ingestion_queue_sem.post();
    this->binary_shutdown_sem.wait();
}