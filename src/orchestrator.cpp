#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "orchestrator.hpp"
#include "io/ingestion.hpp"
#include "core/infrastructure.hpp"
#include "core/executor.hpp"
using namespace std;

// // start clock 
// using nano = std::chrono::nanoseconds;
// auto start = std::chrono::high_resolution_clock::now();
// // stop clock
// auto finish = std::chrono::high_resolution_clock::now();
// std::cout << "Parent run-time: " 
//     << std::chrono::duration_cast<nano>(finish - start).count()
//     << std::endl << std::flush; 

void ingestor_fn(EngineIngestor* ingestor) { 
    ingestor->run_ingestion(); 
}

void executor_fn_1(EngineExecutor* executor) { 
    executor->run_contribution_pipeline(); 
}

void executor_fn_2(EngineExecutor* executor) { 
    executor->run_update_pipeline(); 
}

int main(int argc, const char* argv[]) {

    // pipe descriptors
    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    // run child process
    if (pid == 0) {
        EloStore* elo_store = new EloStore();
        ContributionStore* contribution_store = new ContributionStore(*elo_store);
        EngineIngestor* ingestor = new EngineIngestor(fd[0], fd[1]);
        EngineExecutor* executor = new EngineExecutor(*ingestor, *elo_store, *contribution_store);
        thread t1(ingestor_fn, ingestor);
        thread t2(executor_fn_1, executor);
        thread t3(executor_fn_2, executor);
        sleep(1569325056);
    }

    // run parent process
    else {

        while(true) {
            string result;
            cout << "Enter result ('stop' to end sim): ";
            getline(cin, result);
            if (result == "stop") break;
            write(fd[1], result.c_str(), INGEST_PACKET_BYTES);
        }
        
        close(fd[0]);
        close(fd[1]);
    }

    return 0;
}