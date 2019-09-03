#ifndef MATCH_EXECUTOR_H
#define MATCH_EXECUTOR_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include "core/infrastructure.hpp"
#include "util/semaphore.hpp"
#include "definitions.hpp"

const uint32_t MATCH_QUEUE_REFILL_SIZE = 5;
const uint32_t MATCH_QUEUE_REFILL_LIMIT = 15;
const uint32_t MIN_MATCH_CONTRIBUTION_COUNT = 5;

typedef struct {
    cid contribution_id_a;
    cid contribution_id_b;
} match_t;

/*
    The EngineMatchExecutor class maintains a
    dynamically-updated queue of matches based 
    on the currently stored contribution IDs and 
    their ELO ratings. 
*/
class EngineMatchExecutor : private EngineExecutor {
    public:
        EngineMatchExecutor(EngineContributionStore& contribution_store);
        virtual ~EngineMatchExecutor();
        match_t fetch_match();
        virtual void run();
        virtual void shutdown();

    private:
        std::queue<match_t> match_queue;
        EffSemaphore match_queue_sem;
        std::mutex match_queue_mutex;
        std::condition_variable refill_cv;
        EngineContributionStore& contribution_store;
};

#endif