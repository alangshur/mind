#include "network/ingestion-portal.hpp"
using namespace std;

EngineIngestionPortal::EngineIngestionPortal(uint16_t port, Logger& logger, 
    EngineShutdownOperator& shutdown_operator) : 
    EnginePortalIn(port, logger, shutdown_operator), 
    new_queue(new queue<cid>), update_queue(new queue<pair<cid, cid>>), 
    new_queue_sem(0), update_queue_sem(0), binary_shutdown_sem(0) {}

void EngineIngestionPortal::run_portal() {
    try {
        while (true) {
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineIngestionPortal", "Fatal error in update " 
            "portal" + string(e.what()));
        this->shutdown_operator.signal_node_shutdown();
    }
}