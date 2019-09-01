#include <iostream>
#include "definitions.hpp"
using namespace std;

EffSemaphore global_shutdown_sem(0);

EngineThread::EngineThread() : shutdown_flag(false) {}

void EngineThread::report_fatal_error() {
    global_shutdown_sem.post();
}

void EngineThread::notify_shutdown() {
    this->binary_shutdown_sem.post();
}

void EngineThread::wait_shutdown() {
    this->binary_shutdown_sem.wait();
}