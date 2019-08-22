#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <unistd.h>

// define engine types
typedef int32_t cid;
typedef float elo;

// define engine manager paramaters
const size_t INGEST_PACKET_BYTES = 14;
const size_t INGEST_CID_BYTES = 6;
const char* const SHUTDOWN_PACKET = "000000 000000";

// define engine injector parameters
const elo ELO_INITIAL_RATING = 1000.0;
const float ELO_K_FACTOR = 10.0;
const float ELO_N_SCALE = 400.0;
const float ELO_EXP_BASE = 10.0;

#endif