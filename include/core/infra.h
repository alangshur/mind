#ifndef STORE_H
#define STORE_H

#include <vector>
#include <queue>
#include <atomic>
#include "orchestrator.h"

typedef struct {
    cid contribution_id;
    elo rating;
    int16_t count;
} contribution_t;

/*
    The ELO store is a linear vector-based data structure
    that enumerates a range of ELO scores, with each index 
    containing a priority queue of contribution IDs. This 
    construct allows for the ratings of all the platform 
    contributions to be tediously tracked and organized.
*/
class EloStore {
    public:
        EloStore();
        void add_contribution();
        void update_contribution();
    
    private:
        std::vector<std::atomic<std::priority_queue<cid>>>* store;
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
        std::vector<contribution_t>* store;
};

#endif