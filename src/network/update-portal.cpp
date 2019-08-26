#include "network/update-portal.hpp"
using namespace std;

EngineUpdatePortal::EngineUpdatePortal(uint16_t port, Logger& logger)
    : EnginePortalIn(port, logger), update_queue_sem(0), 
    binary_shutdown_sem(0) {}

void EngineUpdatePortal::run_portal() {
    try {
        while (true) {
            
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineUpdatePortal", "Fatal error in update " 
            "portal" + string(e.what()));
        EngineOrchestrator::signal_node_shutdown(this->logger);
    }
}