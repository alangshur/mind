#ifndef TCPTCPServer_H
#define TCPTCPServer_H

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "orchestrator.hpp"
using boost::asio::ip::tcp;

typedef std::string protocol_t;
typedef std::string payload_t;

typedef struct {
    protocol_t protocol;
    payload_t payload;
} packet_t;

class TCPServer {
    public:
        TCPServer(uint16_t server_port);
        void accept_connection();
        packet_t read_packet();
        packet_t build_packet(const std::string& protocol_str, 
            const std::string& payload_str);
        void write_packet(const packet_t& packet);
        void close_connection();

    private:
        boost::asio::io_context io_context;
        tcp::acceptor acceptor;
        std::shared_ptr<tcp::socket> socket_ptr;
        bool error_flag;
        bool active_connection;
};

class TCPClient {};

#endif