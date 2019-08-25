#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <atomic>
#include "orchestrator.hpp"
#include "io/ingestion.hpp"
#include "util/semaphore.hpp"
#include "core/elo-scoring.hpp"
#include "core/infrastructure.hpp"
#include "util/logger.hpp"

/*
    The engine executor class pieces together the entire 
    input pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineExecutor {
    public:
        EngineExecutor(EngineIngestor& ingestor, EngineEloStore& elo_store, 
            EngineContributionStore& contribution_store, Logger& logger);
        void run_contribution_pipeline();
        void run_update_pipeline();
        void shutdown_pipelines();

    private:
        EngineIngestor& ingestor;
        EngineContributionStore& contribution_store;
        EffSemaphore ternary_shutdown_sem;
        std::atomic<bool> shutdown_flag;
        Logger& logger;
};

#endif