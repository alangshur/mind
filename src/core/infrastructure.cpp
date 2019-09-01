#include <utility>
#include <iostream>
#include "math.h"
#include "core/infrastructure.hpp"
using namespace std;

EngineEloStore::EngineEloStore() : store(ELO_STORE_SIZE) {}

EngineEloStore::~EngineEloStore() {
    for (size_t i = 0; i < store.size(); i++) {
        (this->store)[i].free_list_memory();
    }
}

c_node* EngineEloStore::add_contribution(cid contribution_id, elo init_rating) {
    return (this->store)[(int) init_rating].add_contribution(contribution_id);
}

c_node* EngineEloStore::update_contribution(cid contribution_id, c_node* position, 
    elo old_rating, elo new_rating) {

    // remove old contribution
    (this->store)[(int) round(old_rating)]
        .remove_contribution(position);

    // update contribution
    return (this->store)[(int) round(new_rating)]
        .add_contribution(contribution_id);
}

EngineContributionStore::EngineContributionStore(EngineEloStore& elo_store) : 
    elo_store(elo_store) {}

void EngineContributionStore::add_contribution(cid contribution_id) {
    if ((this->store)[contribution_id].load().contribution_id > 0) {
        throw runtime_error("Addition failed since contribution with ID " 
            + to_string(contribution_id) + " already exists (cannot be added)");
    }

    // build initial contribution
    contribution_t contribution;
    contribution.contribution_id = contribution_id;
    contribution.rating = ELO_INITIAL_RATING;
    contribution.position = this->elo_store.add_contribution(contribution_id, 
        ELO_INITIAL_RATING);

    // store new contribution
    (this->store)[contribution_id].store(contribution);
}

void EngineContributionStore::update_contribution(cid contribution_id, elo new_rating) {
    if ((this->store)[contribution_id].load().contribution_id == 0) {
        throw runtime_error("Updated failed since contribution with ID " 
            + to_string(contribution_id) + " doesn't exist");
    }

    // update old contribution
    contribution_t contribution = (this->store)[contribution_id].load();
    contribution.position = this->elo_store.update_contribution(contribution_id, 
        contribution.position, contribution.rating, new_rating);
    contribution.rating = new_rating;

    // store new contribution
    (this->store)[contribution_id].store(contribution);
}

void EngineContributionStore::remove_contribution(cid contribution_id) {
    if ((this->store)[contribution_id].load().contribution_id == 0) {
        throw runtime_error("Remove failed since contribution with ID " 
            + to_string(contribution_id) + " doesn't exist");
    }

    // remove old contribution
    contribution_t blank_contribution;
    (this->store)[contribution_id].store(blank_contribution);
}

elo EngineContributionStore::fetch_contribution_elo(cid contribution_id) {
    return (this->store)[contribution_id].load().rating;
}