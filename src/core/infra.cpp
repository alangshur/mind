#include <utility>
#include "math.h"
#include "core/infra.hpp"
using namespace std;

EloStore::EloStore() : store(vector<atomic<multimap<int32_t, cid>>>(ELO_STORE_SIZE)) {}

multimap<int32_t, cid>::iterator EloStore::add_contribution(cid contribution_id,
    elo init_rating) {
    
    // insert contribution and return position
    (this->store)[(int) init_rating]
        .load()
        .insert(pair<int32_t, cid>(INITIAL_CONTRIBUTION_COUNT, contribution_id));
}

multimap<int32_t, cid>::iterator EloStore::update_contribution(cid contribution_id, 
    multimap<int32_t, cid>::iterator position, elo old_rating, elo new_rating) {

    // remove old contribution
    (this->store)[(int) round(old_rating)]
        .load()
        .erase(position);

    
}