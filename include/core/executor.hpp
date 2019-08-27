#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <atomic>
#include "orchestrator.hpp"
#include "util/semaphore.hpp"
#include "core/elo-scoring.hpp"
#include "core/infrastructure.hpp"
#include "util/logger.hpp"
#include "network/ingestion-portal.hpp"

/*
    The engine executor class pieces together the entire 
    input pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineExecutor : ThreadedOperator {
    public:
        EngineExecutor(EngineIngestionPortal& ingestor, EngineEloStore& elo_store, 
            EngineContributionStore& contribution_store, Logger& logger, 
            EngineShutdownOperator& shutdown_operator);
        void run_contribution_pipeline();
        void run_update_pipeline();

    private:
        EngineIngestionPortal& ingestor;
        EngineContributionStore& contribution_store;
        EffSemaphore ternary_shutdown_sem;
        Logger& logger;
};

#endif