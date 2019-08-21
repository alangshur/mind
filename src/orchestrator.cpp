#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include "orchestrator.h"
#include "manager/ingestion.h"
using namespace std;

int main(int argc, const char* argv[]) {

    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid == 0) {
        EngineIngestor ei(fd[0], fd[1]);

        // start clock 
        using nano = std::chrono::nanoseconds;
        auto start = std::chrono::high_resolution_clock::now();

        ei.run_ingestion();

        // stop clock
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "Child run-time: " 
            << std::chrono::duration_cast<nano>(finish - start).count()
            << std::endl << std::flush; 
    }
    else {
        
        // start clock 
        using nano = std::chrono::nanoseconds;
        auto start = std::chrono::high_resolution_clock::now();

        write(fd[1], "000123 000321", INGEST_PACKET_BYTES);

        // stop clock
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "Parent run-time: " 
            << std::chrono::duration_cast<nano>(finish - start).count()
            << std::endl << std::flush; 
        
        close(fd[0]);
        close(fd[1]);
    }

    return 0;
}