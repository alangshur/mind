#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <atomic>
#include <mutex>
#include <stdlib.h>
#include "util/logger.hpp"
#include "util/semaphore.hpp"

// define portal ports
const uint16_t INGESTION_PORT = 9000;

// define global types
typedef uint32_t cid;
typedef float elo;
typedef uint8_t node;
typedef uint8_t distribution_rating;

// define global shutdown semaphore
extern EffSemaphore global_shutdown_sem;

/*
    The EngineThread class is a simple parent
    class used to provide base utilities to 
    individually threaded classes (such as portal
    and exec classes).
*/
class EngineThread {        
    public:
        EngineThread();

    protected: 
        void report_fatal_error();
        void notify_shutdown();
        void wait_shutdown();
        virtual void run() = 0;
        virtual void shutdown() = 0;

        Logger logger;
        EffSemaphore binary_shutdown_sem;
        std::atomic<bool> shutdown_flag;
};

/*
    The EngineExecutor class is a simple wrapper
    class that packages the EngineThread class
    for individual executor classes.
*/
class EngineExecutor : protected EngineThread {
    virtual void run() = 0;
    virtual void shutdown() = 0;
};

/*
    The EnginePortal class is a simple wrapper
    class that packages the EngineThread class
    for individual portal classes.
*/
class EnginePortal : protected EngineThread {
    virtual void run() = 0;
    virtual void shutdown() = 0;
};

#endif