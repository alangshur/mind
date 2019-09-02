#include <utility>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "core/infrastructure.hpp"
using namespace std;

EngineEloStore::EngineEloStore() : store(ELO_STORE_SIZE) {}

EngineEloStore::~EngineEloStore() {
    for (size_t i = 0; i < store.size(); i++) {
        (this->store)[i].free_list_memory();
    }
}

c_node* EngineEloStore::add_contribution(cid contribution_id, elo init_rating) {
    return (this->store)[(int) round(init_rating)].add_contribution(contribution_id);
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

void EngineEloStore::remove_contribution(elo rating, c_node* position) {
    (this->store)[(int) round(rating)].remove_contribution(position);
}

uint32_t EngineEloStore::get_rating_list_size(elo rating) {
    return (this->store)[(int) round(rating)].size();
}

cid EngineEloStore::cycle_front_contribution(uint32_t elo_bucket) {
    return (this->store)[elo_bucket].cycle_front_contribution();
}

EngineContributionStore::EngineContributionStore(EngineEloStore& elo_store) : 
    elo_store(elo_store) {}

void EngineContributionStore::add_contribution(cid contribution_id) {

    // build initial contribution
    contribution_t contribution;
    contribution.contribution_id = contribution_id;
    contribution.rating = ELO_INITIAL_RATING;
    contribution.position = this->elo_store.add_contribution(contribution_id, 
        ELO_INITIAL_RATING);

    // store new contribution
    unique_lock<mutex> store_lk(this->store_mutex);
    (this->store)[contribution_id].store(contribution);
    store_lk.unlock();

    // update filled elo bucket list
    unique_lock<mutex> elo_store_lk(this->filled_elo_buckets_mutex);
    this->filled_elo_buckets.insert((int) round(contribution.rating));
}

void EngineContributionStore::update_contribution(cid contribution_id, elo new_rating) {

    // update old contribution
    contribution_t contribution = (this->store)[contribution_id].load();
    contribution.position = this->elo_store.update_contribution(contribution_id, 
        contribution.position, contribution.rating, new_rating);
    elo old_rating = contribution.rating;
    contribution.rating = new_rating;

    // store new contribution
    unique_lock<mutex> store_lk(this->store_mutex);
    (this->store)[contribution_id].store(contribution);
    store_lk.unlock();

    // update filled elo bucket list
    unique_lock<mutex> elo_store_lk(this->filled_elo_buckets_mutex);
    this->filled_elo_buckets.insert((int) round(new_rating));
    if (!this->elo_store.get_rating_list_size(old_rating)) {
        this->filled_elo_buckets.erase((int) round(old_rating));
    }
}

void EngineContributionStore::remove_contribution(cid contribution_id) {

    // remove contribution in elo store
    contribution_t contribution = (this->store)[contribution_id].load();
    this->elo_store.remove_contribution(contribution.rating, contribution.position);

    // remove contribution in contribution store
    unique_lock<mutex> store_lk(this->store_mutex);
    this->store.erase(contribution_id);
    store_lk.unlock();
 
    // update filled elo bucket list
    unique_lock<mutex> elo_store_lk(this->filled_elo_buckets_mutex);
    if (!this->elo_store.get_rating_list_size(contribution.rating)) {
        this->filled_elo_buckets.erase((int) round(contribution.rating));
    }
}

elo EngineContributionStore::fetch_contribution_elo(cid contribution_id) {
    unique_lock<mutex> lk(this->store_mutex);
    return (this->store)[contribution_id].load().rating;
}

bool EngineContributionStore::verify_contribution(cid contribution_id) {
    unique_lock<mutex> lk(this->store_mutex);
    return (this->store).find(contribution_id) != this->store.end();
}

cid EngineContributionStore::attempt_fetch_match_item() {
    unique_lock<mutex> lk(this->filled_elo_buckets_mutex);
    int32_t it_jump = rand() % this->filled_elo_buckets.size();

    // iterate to cid
    auto it = begin(this->filled_elo_buckets);
    advance(it, it_jump);
    return this->elo_store.cycle_front_contribution(*it);
}

pair<cid, cid> EngineContributionStore::fetch_match_pair() {
    pair<cid, cid> match_pair;

    // find base match item
    do { match_pair.first = this->attempt_fetch_match_item(); }
    while (!match_pair.first);

    // find valid additional match item
    do {
        do { match_pair.second = this->attempt_fetch_match_item(); }
        while (!match_pair.second);
    }
    while (match_pair.second != match_pair.first);
    return match_pair;
}