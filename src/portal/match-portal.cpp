#include "portal/match-portal.hpp"
using namespace std;

EnigneMatchPortal::EnigneMatchPortal(EngineMatchExecutor& executor,
    uint16_t port) : executor(executor), server(port) {}
EnigneMatchPortal::~EnigneMatchPortal() {}

void EnigneMatchPortal::run() {
    try {
        while (true) {

            // read new packet
            this->server.accept_connection();
            if (this->shutdown_flag) break;
            match_packet_t match_req;
            this->server.read_packet(match_req);
            if (match_req.request != ACK) 
                throw runtime_error("Invalid request");

            // fetch match
            match_t match = this->executor.fetch_match();

            // write response
            match_packet_t match_res;
            match_res.response = match;
            this->server.write_packet(match_res);
            this->server.close_connection();
        }
    }
    catch(exception& e) {
        this->server.force_close_connection();
        if (!this->shutdown_in_progress()) {
            this->logger.log_error("EnigneMatchPortal", "Fatal error: " 
                + string(e.what()) + ".");
            this->report_fatal_error();
        }
    }

    this->notify_shutdown();
}

void EnigneMatchPortal::shutdown() {
    
    // trigger shutdown
    this->shutdown_flag = true;
    this->server.close_acceptor();

    // wait for shutdown
    this->wait_shutdown();
    this->logger.log_message("EnigneMatchPortal", "Successfully shutdown "
        "match portal.");
}