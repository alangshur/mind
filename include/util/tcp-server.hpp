#ifndef TCPTCPServer_H
#define TCPTCPServer_H

#include <string>
#include "orchestrator.hpp"

enum Protocol {
    MATCH = 1, 
    CHECK = 2,
    INIT = 3
};

typedef char Payload[PAYLOAD_BYTES];
typedef struct {
    Protocol protocol;
    Payload payload;
} Packet;

class TCPServer {
    public:
        TCPServer(uint16_t server_port);
        void run_tcp_server();

    private:
        std::string exec_read();
        void exec_write(std::string message);
};

#endif