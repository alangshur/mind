#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>
#include <string>
#include "orchestrator.hpp"
#include "network/portal.hpp"

static void parse_packet(std::vector<std::string>& payload_tokens, packet_t packet) {
    boost::split(payload_tokens, packet.payload, boost::is_any_of("#"));
}

static packet_t build_packet(const std::string& protocol_str, 
    const std::vector<std::string>& payload_tokens) {
    std::string payload_str = "#";

    // build payload string
    for (int i = 0; i < payload_tokens.size(); i++) {
        payload_str += payload_tokens[i] + "#";
    }

    assert(protocol_str.size() == PROTOCOL_BYTES);
    assert(payload_str.size() <= PAYLOAD_BYTES);
    return {
        protocol_str + std::string(PROTOCOL_BYTES - protocol_str.size(), '-'), 
        payload_str + std::string(PAYLOAD_BYTES - payload_str.size(), '-')
    };
}

// generic definitions
const std::string ACK_PAYLOAD = "#ACK#---------------";
const std::string NAK_PAYLOAD = "#NAK#---------------";

// [START] CONTRIBUTION PROTOCOL
const std::string CONTRIBUTION_PROTOCOL = "CO";

typedef struct {
    node node_num;
    cid contribution_id;
} contribution_req_t;
static contribution_req_t parse_contribution_request(packet_t contribution_request) {
    std::vector<std::string> tokens;
    parse_packet(tokens, contribution_request);
    return {
        static_cast<node>(strtol(tokens[0].c_str(), nullptr, 10)),
        static_cast<cid>(strtol(tokens[1].c_str(), nullptr, 10))
    };
}

typedef packet_t contribution_res_t;
static contribution_res_t build_contribution_response() {
    return build_packet(CONTRIBUTION_PROTOCOL, {ACK_PAYLOAD});
};
// [END] CONTRIBUTION PROTOCOL

// [START] UPDATE PROTOCOL
const std::string UPDATE_PROTOCOL = "UP";

typedef struct {
    node node_num;
    cid contribution_id;
    elo opponent_elo;
} update_req_t;
static update_req_t parse_update_request(packet_t update_request) {
    std::vector<std::string> tokens;
    parse_packet(tokens, update_request);
    return {
        static_cast<node>(strtol(tokens[0].c_str(), nullptr, 10)),
        static_cast<cid>(strtol(tokens[1].c_str(), nullptr, 10)),
        static_cast<elo>(strtof(tokens[2].c_str(), nullptr))
    };
}

typedef packet_t update_res_t;
static update_res_t build_update_response() {
    return build_packet(UPDATE_PROTOCOL, {ACK_PAYLOAD});
};
// [END] UPDATE PROTOCOL

#endif