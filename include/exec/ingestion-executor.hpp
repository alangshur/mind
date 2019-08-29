#ifndef INGESTION_EXECUTOR_H
#define INGESTION_EXECUTOR_H

#include <mutex>
#include <queue>
#include <tuple>
#include "util/semaphore.hpp"
#include "util/elo-scorer.hpp"
#include "core/infrastructure.hpp"
#include "definitions.hpp"

enum ingestion_type {
    Contribution = 1,
    Update = 2,
};

union ingestion_data {
    cid contribution;
    std::tuple<cid, elo, bool> update;
};

typedef struct {
    enum ingestion_type type;
    union ingestion_data data;
} ingestion_t;

/*
    The engine executor class pieces together the entire 
    ingestion pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineIngestionExecutor : private EngineExecutor {
    public:
        EngineIngestionExecutor(EngineContributionStore& contribution_store);
        virtual void run();
        virtual void shutdown();

        std::queue<ingestion_t> ingestion_queue;
        std::mutex ingestion_queue_mutex;
        EffSemaphore ingestion_queue_sem;

    private:
        void handle_contribution(cid contribution);
        elo handle_update(std::tuple<cid, elo, bool>& update);

        EloScorer scorer;
        EngineContributionStore& contribution_store; 
        std::atomic<bool> shutdown_flag;
};

#endif