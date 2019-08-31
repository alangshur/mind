#include "definitions.hpp"

void signal_process_shutdown() { 
    std::unique_lock<std::mutex> lk(global_shutdown_mutex);
    global_shutdown_flag = true; 
    global_shutdown_cv.notify_one();
}

EngineThread::EngineThread() : shutdown_flag(false) {}

void EngineThread::report_fatal_error() {
    signal_process_shutdown();
}

void EngineThread::notify_shutdown() {
    this->binary_shutdown_sem.post();
}

void EngineThread::wait_shutdown() {
    this->binary_shutdown_sem.wait();
}