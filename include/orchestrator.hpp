#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <thread>
#include <vector>
#include "core/infrastructure.hpp"
#include "exec/ingestion-executor.hpp"
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
        void launch_process();
        void wait_process_shutdown();
        void shutdown_process();

        void build_core();
        void build_exec();
        void build_mpi();
        
        // threads
        std::vector<std::thread> mpi_threads;
        std::vector<std::thread> exec_threads;        

        // mpi pointers

        // exec pointers
        EngineIngestionExecutor* ingestion_exec;

        // core pointers
        EngineEloStore* elo_store;
        EngineContributionStore* contribution_store;
};

#endif