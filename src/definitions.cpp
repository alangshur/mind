#include "definitions.hpp"

void signal_process_shutdown() { 
    std::unique_lock<std::mutex> lk(shutdown_mutex);
    shutdown_flag = true; 
    shutdown_cv.notify_one();
}

void EngineThread::report_fatal_error() {
    signal_process_shutdown();
}