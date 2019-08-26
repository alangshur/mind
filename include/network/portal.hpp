#ifndef PORTAL_H
#define PORTAL_H

#include "util/logger.hpp"
#include "util/tcp-server.hpp"

/*
    The engine portal class is a top-level parent class 
    aimed at defining a simple foundation for various 
    types of node network portal categories.
*/
class EnginePortal {
    public:
        virtual void run_portal() = 0;

    protected:
        EnginePortal(Logger& logger, EngineShutdownOperator& shutdown_operator)
            : logger(logger), shutdown_operator(shutdown_operator) {}
            
        Logger& logger;
        EngineShutdownOperator& shutdown_operator;
};

/*
    The engine portal-in class is a second-level parent 
    class that further narrows the function of a specific 
    node network portal. Specifically, this class defines a
    foundation for server-portals.  
*/
class EnginePortalIn : protected EnginePortal {            
    protected:
        EnginePortalIn(uint16_t port, Logger& logger, 
            EngineShutdownOperator& shutdown_operator) 
            : EnginePortal(logger, shutdown_operator), server(port){}
        virtual void run_portal() = 0;
        void send_ack_packet(std::string protocol) 
            { this->server.write_packet({protocol, ACK_PAYLOAD}); };
        void send_nak_packet(std::string protocol) 
            { this->server.write_packet({protocol, NAK_PAYLOAD}); };

        TCPServer server;
};

/*
    The engine portal-out class is a second-level parent 
    class that further narrows the function of a specific 
    node network portal. Specifically, this class defines a
    foundation for client-portals.  
*/
class EnginePortalOut : protected EnginePortal {
    protected:
        EnginePortalOut(Logger& logger, EngineShutdownOperator& shutdown_operator)
            : EnginePortal(logger, shutdown_operator) {}
        virtual void run_portal() = 0;
        void send_ack_packet(std::string protocol) 
            { this->client.write_packet({protocol, ACK_PAYLOAD}); };
        void send_nak_packet(std::string protocol) 
            { this->client.write_packet({protocol, NAK_PAYLOAD}); };

        TCPClient client;
};

#endif