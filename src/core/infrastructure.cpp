#include <utility>
#include <iostream>
#include "math.h"
#include "core/infrastructure.hpp"
using namespace std;

EloStore::EloStore() : store(vector<EloList>(ELO_STORE_SIZE)) {}

EloStore::~EloStore() {
    for (size_t i = 0; i < store.size(); i++) {
        (this->store)[i].free_list_memory();
    }
}

c_node* EloStore::add_contribution(cid contribution_id, elo init_rating) {
    
    // insert contribution and return position
    return (this->store)[(int) init_rating].add_contribution(contribution_id);
}

c_node* EloStore::update_contribution(cid contribution_id, c_node* position, 
    elo old_rating, elo new_rating) {

    // remove old contribution
    (this->store)[(int) round(old_rating)]
        .remove_contribution(position);

    // update contribution
    return (this->store)[(int) round(new_rating)]
        .add_contribution(contribution_id);
}

ContributionStore::ContributionStore(EloStore& elo_store) : elo_store(elo_store),
    store(vector<atomic<contribution_t>>(CONTRIBUTION_STORE_SIZE)) {}

void ContributionStore::add_contribution(cid contribution_id) {

    // build initial contribution
    contribution_t contribution;
    contribution.contribution_id = contribution_id;
    contribution.rating = ELO_INITIAL_RATING;
    contribution.position = this->elo_store.add_contribution(contribution_id, 
        ELO_INITIAL_RATING);

    // store new contribution
    (this->store)[contribution_id].store(contribution);
}

void ContributionStore::update_contribution(cid contribution_id, elo new_rating) {

    // update old contribution
    contribution_t contribution = (this->store)[contribution_id].load();
    contribution.position = this->elo_store.update_contribution(contribution_id, 
        contribution.position, contribution.rating, new_rating);
    contribution.rating = new_rating;

    // store new contribution
    (this->store)[contribution_id].store(contribution);
}

elo ContributionStore::fetch_contribution_elo(cid contribution_id) {
    return (this->store)[contribution_id].load().rating;
}