#ifndef UPDATE_PORTAL_H
#define UPDATE_PORTAL_H

#include <queue>
#include <atomic>
#include "util/logger.hpp"
#include "util/semaphore.hpp"
#include "portal.hpp"
#include "orchestrator.hpp"
 
/*
    The engine update portal class is an engine 
    portal-in variant portal that controls the 
    node's UPDATE protocol processses.
*/
class EngineUpdatePortal : private EnginePortalIn {
    public:
        EngineUpdatePortal(uint16_t port, Logger& logger);
        ~EngineUpdatePortal();
        void run_portal(); 

    private:
        std::atomic<std::queue<std::pair<cid, cid>>*> update_queue;
        EffSemaphore update_queue_sem;
        EffSemaphore binary_shutdown_sem;
};

#endif