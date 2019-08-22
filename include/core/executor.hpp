#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <atomic>
#include "orchestrator.hpp"
#include "manager/ingestion.hpp"
#include "manager/semaphore.hpp"
#include "core/scoring.hpp"
#include "core/infra.hpp"

/*
    The engine executor class pieces together the entire 
    input pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineExecutor {
    public:
        EngineExecutor(EngineIngestor& ingestor, EloStore& elo_store, 
            ContributionStore& contribution_store);
        void run_contribution_pipeline();
        void run_update_pipeline();
        void shutdown_pipelines();

    private:
        EngineIngestor& ingestor;
        ContributionStore& contribution_store;
        EffSemaphore ternary_shutdown_sem;
        std::atomic<bool> shutdown_flag;
};

#endif