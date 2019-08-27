#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <string>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <thread>
#include "util/logger.hpp"

// define engine types
typedef uint32_t cid;
typedef float elo;
typedef uint8_t node;

// define network parameters
const uint32_t PROTOCOL_BYTES = 2;
const uint32_t PAYLOAD_BYTES = 20;
const uint32_t PACKET_BYTES = PROTOCOL_BYTES + PAYLOAD_BYTES;

// define ELO parameters
const elo ELO_INITIAL_RATING = 1000.0;
const float ELO_K_FACTOR = 20.0;
const float ELO_N_SCALE = 400.0;
const float ELO_EXP_BASE = 10.0;

// define strucutre parameters
const uint32_t ELO_STORE_SIZE = 10000;
const uint32_t CONTRIBUTION_STORE_SIZE = 10000;
const uint32_t INITIAL_CONTRIBUTION_COUNT = 0;

class EngineShutdownOperator {
    public:
        EngineShutdownOperator(Logger& logger) : logger(logger) {}
        void signal_node_shutdown() {
            std::lock_guard<std::mutex> lg(shutdown_lock);
            this->logger.log_error("EngineOrchestrator", "Terminating node.");
            this->shutdown_node();
        };

    private:
        void shutdown_node() { exit(0); }
        
        std::mutex shutdown_lock;
        Logger& logger;
};

class ThreadedOperator {
    public:
        ThreadedOperator(EngineShutdownOperator& shutdown_operator)
            : shutdown_operator(shutdown_operator) {}

    protected:
        EngineShutdownOperator& shutdown_operator;
};

class EngineOrchestrator {
    public:
        EngineOrchestrator(node node_num) : 
            node_num(node_num),
            logger(node_num), 
            shutdown_operator(this->logger) {}
        void orchestrator_begin() {
            this->logger.log_message("EngineOrchestrator", "Launching node " + 
                std::to_string(this->node_num) + ".");
            // TODO: launch node
        }

    private:
        node node_num;
        Logger logger;
        EngineShutdownOperator shutdown_operator;
};

#endif