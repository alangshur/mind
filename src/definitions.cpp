#include <iostream>
#include "definitions.hpp"
using namespace std;

EffSemaphore global_shutdown_sem(0);
atomic<bool> global_shutdown_flag(false);

EngineThread::EngineThread() : shutdown_flag(false) {}

void EngineThread::report_fatal_error() {
    global_shutdown_sem.post();
    global_shutdown_flag = true;
}

bool EngineThread::shutdown_in_progress() {
    return global_shutdown_flag;
}

void EngineThread::notify_shutdown() {
    this->binary_shutdown_sem.post();
}

void EngineThread::wait_shutdown() {
    this->binary_shutdown_sem.wait();
}