#ifndef STORE_H
#define STORE_H

#include <vector>
#include <queue>
#include "orchestrator.h"

typedef struct {
    cid contribution_id;
    elo rating;
    int16_t count;
} contribution_t;

class EloStore {
    public:
        void add_contribution();
        void update_contribution();
    
    private:
        std::vector<std::priority_queue<cid>> store;
};

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