#include "core/infra.hpp"
using namespace std;

EloStore::EloStore() : store(new vector<atomic<priority_queue<cid>>>(ELO_STORE_SIZE)) {}