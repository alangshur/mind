#ifndef INGESTION_EXECUTOR_H
#define INGESTION_EXECUTOR_H

#include <atomic>
#include <queue>
#include <tuple>
#include "util/semaphore.hpp"
#include "util/elo-scorer.hpp"
#include "core/infrastructure.hpp"
#include "definitions.hpp"

/*
    The engine executor class pieces together the entire 
    ingestion pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineIngestionExecutor : private EngineThread {
    public:
        EngineIngestionExecutor(EloScorer& scorer, 
            EngineContributionStore& contribution_store);
        ~EngineIngestionExecutor();
        void run_contribution_pipeline();
        void run_update_pipeline();

    private:
        EloScorer& scorer;
        EngineContributionStore& contribution_store;
        EffSemaphore ternary_shutdown_sem;
        std::atomic<std::queue<cid>*> new_queue;
        std::atomic<std::queue<std::tuple<cid, elo, bool>>*> update_queue;
        EffSemaphore new_queue_sem;
        EffSemaphore update_queue_sem;   
};

#endif