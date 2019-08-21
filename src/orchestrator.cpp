#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include "orchestrator.h"
#include "manager/semaphore.h"

int main(int argc, const char* argv[]) {

    // start clock 
    using nano = std::chrono::nanoseconds;
    auto start = std::chrono::high_resolution_clock::now();

    EffSemaphore sem(5);
    sem.post();
    sem.wait();

    // stop clock
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "Total application run-time: " 
        << std::chrono::duration_cast<nano>(finish - start).count()
        << std::endl << std::flush; 
    return 0;
}