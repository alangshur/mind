#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H

#include <mutex>
#include <set>
#include <map>
#include <vector>
#include <utility>
#include <atomic>
#include "util/rating-list.hpp"
#include "definitions.hpp"

const elo ELO_INITIAL_RATING = 1000.0;
const uint32_t ELO_STORE_SIZE = 10000;
const uint32_t INITIAL_CONTRIBUTION_COUNT = 0;

typedef struct {
    cid contribution_id = 0;
    elo rating = 0;
    c_node* position = nullptr;
} contribution_t;

/*
    The EngineEloStore class is a linear vector-based data structure
    that enumerates a range of ELO scores, with each index 
    containing a linked-list of contribution IDs. This construct 
    allows for the ratings of all the platform contributions 
    to be tediously tracked and organized.
*/
class EngineEloStore {
    friend class EngineContributionStore;

    public:
        EngineEloStore();
        ~EngineEloStore();

    private:
        c_node* add_contribution(cid contribution_id, elo init_rating);
        c_node* update_contribution(cid contribution_id, 
            c_node* position, elo old_rating, elo new_rating);
        void remove_contribution(elo rating, c_node* position);
        uint32_t get_rating_list_size(elo rating);
        cid cycle_front_contribution(uint32_t elo_bucket);
            
        std::vector<RatingList> store;
};

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

    private:    
        EngineEloStore& elo_store;
        std::map<cid, std::atomic<contribution_t>> store;
        std::mutex store_mutex;
        std::set<uint32_t> filled_elo_buckets;
        std::mutex filled_elo_buckets_mutex;
        std::atomic<uint32_t> contribution_count;
};

#endif