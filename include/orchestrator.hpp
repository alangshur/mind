#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <thread>
#include "core/infrastructure.hpp"
#include "definitions.hpp"

/*
    The engine orchestrator class is the central 
    orchestration unit for a single node. It manages
    the creation of all the individual modules, as well
    as assigning them to threads and monitoring these 
    child threads.
*/
class EngineOrchestrator : EngineThread {
    public:
        EngineOrchestrator();
        ~EngineOrchestrator();
        void execute();

    private:
        void launch_node();
        void wait_node_shutdown();
        void shutdown_node();

        void build_core();
        void build_exec();
        void build_mpi();

        EngineEloStore* elo_store;
        EngineContributionStore* contribution_store;
};

#endif