#ifndef INGESTION_PORTAL_H
#define INGESTION_PORTAL_H

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
class EngineIngestionPortal : private EnginePortalIn {
    public:
        EngineIngestionPortal(uint16_t port, Logger& logger,
            EngineShutdownOperator& shutdown_operator);
        ~EngineIngestionPortal();
        void run_portal(); 

        std::atomic<std::queue<cid>*> new_queue;
        std::atomic<std::queue<std::pair<cid, elo>>*> update_queue;
        EffSemaphore new_queue_sem;
        EffSemaphore update_queue_sem;        
};

#endif