#include <vector>
#include "network/protocol.hpp"
#include "network/ingestion-portal.hpp"
using namespace std;

EngineIngestionPortal::EngineIngestionPortal(uint16_t port, Logger& logger, 
    EngineShutdownOperator& shutdown_operator) : 
    EnginePortalIn(port, logger, shutdown_operator), 
    new_queue(new queue<cid>), update_queue(new queue<pair<cid, elo>>), 
    new_queue_sem(0), update_queue_sem(0) {}

EngineIngestionPortal::~EngineIngestionPortal() { 
    delete this->new_queue.load();
    delete this->update_queue.load();
}

void EngineIngestionPortal::run_portal() {
    try {
        while (true) {

            // read new packet
            this->server.accept_connection();
            packet_t packet = this->server.read_packet();

            // add packet contents to queue
            if (packet.protocol == CONTRIBUTION_PROTOCOL) {
                contribution_req_t req = parse_contribution_request(packet);
                this->new_queue.load()->push(req.contribution_id);
                this->new_queue_sem.post();
                this->server.write_packet(build_contribution_response());
            }
            else if (packet.protocol == UPDATE_PROTOCOL) {
                update_req_t req = parse_update_request(packet);
                this->update_queue.load()->push(pair<cid, elo>(req.contribution_id, req.opponent_elo));
                this->update_queue_sem.post();
                this->server.write_packet(build_update_response());
            }
            else throw runtime_error("Invalid protocol \"" + packet.protocol
                + "\" in ingestion portal.");

            // close connection
            this->server.close_connection();
        }
    }
    catch(exception& e) {
        this->logger.log_error("EngineIngestionPortal", "Fatal error in update " 
            "portal" + string(e.what()));
        this->shutdown_operator.signal_node_shutdown();
    }
}