#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <unistd.h>

// define engine types
typedef int32_t cid;
typedef int16_t elo;

// define engine manager paramaters
const size_t INGEST_PACKET_BYTES = 14;
const size_t INGEST_CID_BYTES = 6;
const char* const SHUTDOWN_PACKET = "000000 000000";

#endif