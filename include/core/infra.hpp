#ifndef STORE_H
#define STORE_H

#include <vector>
#include <list>
#include <atomic>
#include "orchestrator.hpp"

typedef struct {
    cid contribution_id;
    elo rating;
    std::list<cid>::iterator position;
} contribution_t;

/*
    The ELO store is a linear vector-based data structure
    that enumerates a range of ELO scores, with each index 
    containing a linked-list of contribution IDs. This construct 
    allows for the ratings of all the platform contributions 
    to be tediously tracked and organized.
*/
class EloStore {
    friend class ContributionStore;

    public:
        EloStore();
        ~EloStore();

    private:
        std::list<cid>::iterator add_contribution(cid contribution_id, elo init_rating);
        std::list<cid>::iterator update_contribution(cid contribution_id, 
            std::list<cid>::iterator position, elo old_rating, elo new_rating);
        std::vector<std::atomic<std::list<cid>>> store;
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
        ~ContributionStore();
        void add_contribution(cid contribution_id);
        void update_contribution(cid contribution_id, elo new_rating);

    private:
        EloStore& elo_store;
        std::vector<std::atomic<contribution_t>> store;
};

#endif