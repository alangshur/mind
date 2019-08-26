#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <string>
#include <unistd.h>
#include <mutex>
#include "util/logger.hpp"

// define engine types
typedef uint32_t cid;
typedef float elo;

// define network parameters
const uint32_t PROTOCOL_BYTES = 2;
const uint32_t PAYLOAD_BYTES = 14;
const uint32_t PACKET_BYTES = PROTOCOL_BYTES + PAYLOAD_BYTES;

// define packet definitions
const std::string CONTR_PROTOCOL = "CO";
const std::string UPDATE_PROTOCOL = "UP";
const std::string ACK_PAYLOAD = "ACK-----------";
const std::string NAK_PAYLOAD = "NAK-----------";

// define manager paramaters
const uint32_t INGEST_PACKET_BYTES = 14;
const uint32_t INGEST_CID_BYTES = 6;
const char* const SHUTDOWN_PACKET = "000000 000000";

// define ELO parameters
const elo ELO_INITIAL_RATING = 1000.0;
const float ELO_K_FACTOR = 20.0;
const float ELO_N_SCALE = 400.0;
const float ELO_EXP_BASE = 10.0;

// define strucutre parameters
const uint32_t ELO_STORE_SIZE = 5000;
const uint32_t CONTRIBUTION_STORE_SIZE = 10000;
const uint32_t INITIAL_CONTRIBUTION_COUNT = 0;

static std::mutex shutdown_lock;
class EngineOrchestrator {
    public:
        static void signal_node_shutdown(Logger& logger) {
            std::lock_guard<std::mutex> lg(shutdown_lock);
            logger.log_error("EngineOrchestrator", "Terminating node.");
        };
};

#endif