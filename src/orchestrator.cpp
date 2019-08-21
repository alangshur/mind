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
        ei.run_ingestion();
    }
    else {

        // start clock 
        using nano = std::chrono::nanoseconds;
        auto start = std::chrono::high_resolution_clock::now();

        while(true) {
            string result;
            cout << "Enter result ('stop' to end sim): ";
            getline(cin, result);
            if (result == "stop") break;
            write(fd[1], result.c_str(), INGEST_PACKET_BYTES);
        }
        
        close(fd[0]);
        close(fd[1]);

        // stop clock
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "Parent run-time: " 
            << std::chrono::duration_cast<nano>(finish - start).count()
            << std::endl << std::flush; 
    }

    return 0;
}