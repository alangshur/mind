#ifndef MATCH_PORTAL_H
#define MATCH_PORTAL_H

#include "util/tcp-server.hpp"
#include "exec/match-executor.hpp"
#include "definitions.hpp"

enum packet_request {
    ACK = 1,
    NAK = 2
};

typedef union {
    enum packet_request request;
    match_t response;
} match_packet_t;

/*
    The EnigneMatchPortal class is a portal mounted
    on top of a TCP server to read match data and 
    feed the result into the match executor. 
*/
class EngineMatchPortal : private EnginePortal {
    public:
        EngineMatchPortal(EngineMatchExecutor& executor, uint16_t port);
        virtual ~EngineMatchPortal();
        virtual void run();
        virtual void shutdown();

    private:
        EngineMatchExecutor& executor;
        TCPServer<match_packet_t> server;
};

#endif