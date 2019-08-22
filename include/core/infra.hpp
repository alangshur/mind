#ifndef STORE_H
#define STORE_H

#include <vector>
#include <atomic>
#include "core/elolist.hpp"
#include "orchestrator.hpp"

typedef struct {
    cid contribution_id;
    elo rating;
    c_node* position;
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