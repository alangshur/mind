#include <utility>
#include "math.h"
#include "core/infra.hpp"
using namespace std;

EloStore::EloStore() : store(vector<atomic<multimap<size_t, cid>>>(ELO_STORE_SIZE)) {}

multimap<size_t, cid>::iterator EloStore::add_contribution(cid contribution_id,
    elo init_rating) {
    
    // insert contribution and return position
    return (this->store)[(int) init_rating]
        .load()
        .insert(pair<size_t, cid>(INITIAL_CONTRIBUTION_COUNT, contribution_id));
}

multimap<size_t, cid>::iterator EloStore::update_contribution(cid contribution_id, 
    multimap<size_t, cid>::iterator position, elo old_rating, elo new_rating,
    size_t count) {

    // remove old contribution
    (this->store)[(int) round(old_rating)]
        .load()
        .erase(position);

    // update contribution
    return (this->store)[(int) round(new_rating)]
        .load()
        .insert(pair<size_t, cid>(count, contribution_id));
}