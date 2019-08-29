#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <condition_variable>
#include <mutex>
#include <stdlib.h>
#include "util/logger.hpp"
#include "util/semaphore.hpp"

typedef uint32_t cid;
typedef float elo;
typedef uint8_t node;
typedef uint8_t distribution_rating;

static std::mutex shutdown_mutex;
static std::condition_variable shutdown_cv;
static bool shutdown_flag = false;
void signal_process_shutdown();

/*
    The engine thread class is a simple parent
    class used to provide base utilities to 
    individually threaded classes (such as portal
    and exec classes).
*/
class EngineThread {        
    protected: 
        void report_fatal_error();
        void wait_shutdown();
        virtual void run() = 0;
        virtual void shutdown() = 0;

        Logger logger;
        EffSemaphore binary_shutdown_sem;
};

class EngineExecutor : protected EngineThread {
    virtual void run() = 0;
    virtual void shutdown() = 0;
};

class EnginePortal : protected EngineThread {
    virtual void run() = 0;
    virtual void shutdown() = 0;
};

#endif