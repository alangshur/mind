#include "exec/match-executor.hpp"
using namespace std;

EngineMatchExecutor::EngineMatchExecutor(EngineContributionStore& contribution_store) 
    : contribution_store(contribution_store) {
    this->logger.log_message("EngineMatchExecutor", "Initializing match executor.");
}

EngineMatchExecutor::~EngineMatchExecutor() {}

match_t EngineMatchExecutor::fetch_match() {
    this->match_queue_sem.wait();

    // fetch new match
    std::unique_lock lk(this->match_queue_mutex);
    match_t match = this->match_queue.front();
    this->match_queue.pop();
    this->refill_cv.notify_one();
    return match;
}

void EngineMatchExecutor::run() {
    try {
        while (true) {  

            // wait for refill
            unique_lock<mutex> lk(this->match_queue_mutex);
            this->refill_cv.wait(lk, [&, this]() { 
                return ((this->match_queue.size() <= MATCH_QUEUE_REFILL_SIZE)
                    && (this->contribution_store.get_contribution_count() 
                    >= MIN_MATCH_CONTRIBUTION_COUNT)) || this->shutdown_flag; 
            });
            if (this->shutdown_flag) break;

            // refill matches
            while (this->match_queue.size() < MATCH_QUEUE_REFILL_LIMIT) {
                pair<cid, cid> match_pair = this->contribution_store.fetch_match_pair();
                this->match_queue.push({ match_pair.first, match_pair.second });
                this->match_queue_sem.post();
            }   
        }
    }
    catch(exception& e) {
        if (!this->shutdown_in_progress()) {
            this->logger.log_error("EngineMatchExecutor", "Fatal error: " 
                + string(e.what()) + ".");
            this->report_fatal_error();
        }
    }

    this->notify_shutdown();
}

void EngineMatchExecutor::shutdown() {
    
    // trigger shutdown
    this->shutdown_flag = true;
    this->refill_cv.notify_all();

    // wait for shutdown
    this->wait_shutdown();
    this->logger.log_message("EngineMatchExecutor", "Successfully shutdown "
        "match executor.");
}
