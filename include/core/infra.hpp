#ifndef STORE_H
#define STORE_H

#include <vector>
#include <map>
#include <atomic>
#include "orchestrator.hpp"

typedef struct {
    cid contribution_id;
    elo rating;
    int32_t count;
    std::multimap<int32_t, cid>::iterator position;
} contribution_t;

/*
    The ELO store is a linear vector-based data structure
    that enumerates a range of ELO scores, with each index 
    containing a multimap of contribution IDs. This construct 
    allows for the ratings of all the platform contributions 
    to be tediously tracked and organized.
*/
class EloStore {
    public:
        EloStore();
        std::multimap<int32_t, cid>::iterator add_contribution(cid contribution_id, 
            elo init_rating);
        std::multimap<int32_t, cid>::iterator update_contribution(cid contribution_id, 
            std::multimap<int32_t, cid>::iterator position, elo old_rating, elo new_rating);
    
    private:
        std::vector<std::atomic<std::multimap<int32_t, cid>>> store;
};

/*
    The contribution store is a linear vector-based data
    structure mounted on top of the ELO store described above.
    This system provides external indexing capabilities to 
    the store above to allow for individual contributions to be
    manually indexed. 
*/
class ContributionStore {
    public:
        ContributionStore(EloStore& elo_store);
        void add_contribution();
        void update_contribution();

    private:
        EloStore& elo_store;
        std::vector<contribution_t> store;
};

#endif