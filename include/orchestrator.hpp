#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <unistd.h>

// define engine types
typedef size_t cid;
typedef float elo;

// define manager paramaters
const size_t INGEST_PACKET_BYTES = 14;
const size_t INGEST_CID_BYTES = 6;
const char* const SHUTDOWN_PACKET = "000000 000000";

// define ELO parameters
const elo ELO_INITIAL_RATING = 1000.0;
const float ELO_K_FACTOR = 4.0;
const float ELO_N_SCALE = 400.0;
const float ELO_EXP_BASE = 10.0;

// define strucutre parameters
const size_t ELO_STORE_SIZE = 5000;
const size_t CONTRIBUTION_STORE_SIZE = 100000;
const size_t INITIAL_CONTRIBUTION_COUNT = 0;

#endif