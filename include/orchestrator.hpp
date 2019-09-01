#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <thread>
#include <vector>
#include "util/logger.hpp"
#include "core/infrastructure.hpp"
#include "exec/ingestion-executor.hpp"
#include "portal/ingestion-portal.hpp"
#include "definitions.hpp"

/*
    The EngineOrchestrator class is the central 
    orchestration unit for a single node. It manages
    the creation of all the individual modules, as well
    as assigning them to threads and monitoring these 
    child threads.
*/
class EngineOrchestrator {
    public:
        EngineOrchestrator();
        ~EngineOrchestrator();
        void execute();

    private:
        void launch_process();
        void wait_process_shutdown();
        void shutdown_process();

        void build_core();
        void build_exec();
        void build_portal();      

        void shutdown_portal();
        void shutdown_exec();

        Logger logger;

        // core pointers
        EngineEloStore* elo_store;
        EngineContributionStore* contribution_store;

        // exec pointers
        EngineIngestionExecutor* ingestion_exec;

        // portal pointers
        EngineIngestionPortal* ingestion_portal;

        // threads
        std::vector<std::thread> portal_threads;
        std::vector<std::thread> exec_threads; 
};

#endif