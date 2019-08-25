#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H

#include <vector>
#include <atomic>
#include "util/rating-list.hpp"
#include "orchestrator.hpp"

typedef struct {
    cid contribution_id = 0;
    elo rating = 0;
    c_node* position = nullptr;
} contribution_t;

/*
    The ELO store is a linear vector-based data structure
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
        std::vector<EloList> store;
};

/*
    The contribution store is a linear vector-based data
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
        elo fetch_contribution_elo(cid contribution_id);

    private:
        EngineEloStore& elo_store;
        std::vector<std::atomic<contribution_t>> store;
};

#endif