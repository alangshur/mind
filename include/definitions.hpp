#ifndef NODE_PARAMS_H
#define NODE_PARAMS_H

#include <condition_variable>
#include <mutex>
#include <stdlib.h>
#include "util/logger.hpp"

typedef uint32_t cid;
typedef float elo;
typedef uint8_t node;
typedef uint8_t distribution_rating;

static std::mutex shutdown_mutex;
static std::condition_variable shutdown_cv;
static bool shutdown_flag = false;
void signal_node_shutdown();

/*
    The engine thread class is a simple parent
    class used to provide base utilities to 
    individually threaded classes (such as mpi
    and exec classes).
*/
class EngineThread {
    protected: 
        Logger logger;
        void report_fatal_error();
        virtual void shutdown() = 0;
};

#endif