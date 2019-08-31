#ifndef INGESTION_EXECUTOR_H
#define INGESTION_EXECUTOR_H

#include <mutex>
#include <queue>
#include "util/semaphore.hpp"
#include "util/elo-scorer.hpp"
#include "core/infrastructure.hpp"
#include "definitions.hpp"

enum ingestion_type {
    Contribution = 1,
    Update = 2,
};

typedef struct {
    cid contribution_id;
} ingestion_contribution_t;  

typedef struct {
    cid contribution_id;
    elo opponent_rating;
    bool is_winner;
} ingestion_update_t;

union ingestion_data {
    ingestion_contribution_t contribution;
    ingestion_update_t update;
};

typedef struct {
    enum ingestion_type type;
    union ingestion_data data;
} ingestion_t;

/*
    The EngineIngestionExecutor class pieces together the entire 
    ingestion pipeline for the engine algorithm. It fetches data 
    (contributions and updates) that have been ingested and 
    injects them into the stores framework defined within the 
    engine core after running them through the ELO rating module.
*/
class EngineIngestionExecutor : private EngineExecutor {
    public:
        EngineIngestionExecutor(EngineContributionStore& contribution_store);
        virtual ~EngineIngestionExecutor();
        void run();
        void shutdown();

        std::queue<ingestion_t> ingestion_queue;
        std::mutex ingestion_queue_mutex;
        EffSemaphore ingestion_queue_sem;

    private:
        void handle_contribution(ingestion_contribution_t& contribution);
        elo handle_update(ingestion_update_t& update);

        EloScorer scorer;
        EngineContributionStore& contribution_store; 
};

#endif