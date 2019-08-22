#include <utility>
#include "math.h"
#include "core/infra.hpp"
using namespace std;

// EloStore::EloStore() : store(vector<atomic<list<cid>>>(ELO_STORE_SIZE)) {}

// list<cid>::iterator EloStore::add_contribution(cid contribution_id, elo init_rating) {
    
//     // insert contribution and return position
//     (this->store)[(int) init_rating]
//         .load()
//         .push_back(contribution_id);
//     return 
// }

// list<cid>::iterator EloStore::update_contribution(cid contribution_id, 
//     list<cid>::iterator position, elo old_rating, elo new_rating) {

//     // remove old contribution
//     (this->store)[(int) round(old_rating)]
//         .load()
//         .erase(position);

//     // update contribution
//     return (this->store)[(int) round(new_rating)]
//         .load()
//         .insert(pair<uint32_t, cid>(count, contribution_id));
// }

// ContributionStore::ContributionStore(EloStore& elo_store) : elo_store(elo_store),
//     store(vector<atomic<contribution_t>>(CONTRIBUTION_STORE_SIZE)) {}

// void ContributionStore::add_contribution(cid contribution_id) {

//     // build initial contribution
//     contribution_t contribution;
//     contribution.contribution_id = contribution_id;
//     contribution.rating = ELO_INITIAL_RATING;
//     contribution.count = INITIAL_CONTRIBUTION_COUNT;
//     contribution.position = this->elo_store.add_contribution(contribution_id, 
//         ELO_INITIAL_RATING, INITIAL_CONTRIBUTION_COUNT);

//     // store new contribution
//     (this->store)[contribution_id].store(contribution);
// }

// void ContributionStore::update_contribution(cid contribution_id, elo new_rating) {

//     // update old contribution
//     contribution_t contribution = (this->store)[contribution_id].load();
//     contribution.position = this->elo_store.update_contribution(contribution_id, 
//         contribution.position, contribution.rating, new_rating, contribution.count);
//     contribution.rating = new_rating;

//     // store new contribution
//     (this->store)[contribution_id].store(contribution);
// }