#ifndef INGESTION_PORTAL_H
#define INGESTION_PORTAL_H

#include "util/tcp-server.hpp"
#include "exec/ingestion-executor.hpp"
#include "definitions.hpp"

enum packet_response {
    ACK = 1,
    NAK = 2
};

typedef union {
    ingestion_t request;
    enum packet_response response;
} ingestion_packet_t;

/*
    The EngineIngestionPortal class is a portal mounted
    on top of a TCP server to read contribution and update
    data and feed the result into the ingestion executor. 
*/
class EngineIngestionPortal : private EnginePortal {
    public:
        EngineIngestionPortal(EngineIngestionExecutor& executor, uint16_t port);
        virtual void run();
        virtual void shutdown();

    private:
        EngineIngestionExecutor& executor;
        TCPServer<ingestion_packet_t> server;
};

#endif