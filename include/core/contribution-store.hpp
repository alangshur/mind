#ifndef CONTRIBUTION_STORE_H
#define CONTRIBUTION_STORE_H

#include <mutex>
#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <atomic>
#include "core/elo-store.hpp"
#include "util/rating-list.hpp"
#include "util/iqr-scorer.hpp"
#include "util/semaphore.hpp"
#include "definitions.hpp"

const elo ELO_INITIAL_RATING = 1000.0;
const uint32_t INITIAL_CONTRIBUTION_COUNT = 0;

typedef struct {
    cid contribution_id = 0;
    elo rating = 0;
    c_node* position = nullptr;
} contribution_t;

/*
    The EngineContributionStore class is a red-black-tree data
    structure mounted on top of the ELO store described above.
    This system provides external indexing capabilities to 
    the store above to allow for individual contributions to be
    manually indexed. 
*/
class EngineContributionStore {
    public:
        EngineContributionStore(EngineEloStore& elo_store);
        void add_contribution(cid contribution_id);
        void update_contribution(cid contribution_id, elo new_rating);
        void remove_contribution(cid contribution_id);
        elo fetch_contribution_elo(cid contribution_id);
        bool verify_contribution(cid contribution_id);
        cid attempt_fetch_match_item();
        std::pair<cid, cid> fetch_match_pair();
        uint32_t get_contribution_count();
        cid dump_above_outlier_until();
        cid dump_below_outlier_until();

    private:    
        EngineEloStore& elo_store;
        std::atomic<uint32_t> contribution_count;

        std::map<cid, std::atomic<contribution_t>> store;
        std::mutex store_mutex;

        IQRScorer elo_bucket_scorer;
        std::mutex elo_bucket_scorer_mutex;

        std::queue<cid> above_outlier_queue;
        std::mutex above_outlier_queue_mutex;

        std::queue<cid> below_outlier_queue;
        std::mutex below_outlier_queue_mutex;

};

#endif