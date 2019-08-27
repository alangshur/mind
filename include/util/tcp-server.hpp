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

/*
    The TCP server class is a synchronous TCP framework for
    sending/receiving server TCP connections and sending/receiving 
    server packet communications over these connections.
*/
class TCPServer {
    public:
        TCPServer(uint16_t server_port);
        void accept_connection();
        packet_t read_packet();
        void write_packet(const packet_t& packet);
        void close_connection();
        
    private:
        boost::asio::io_context io_context;
        tcp::acceptor acceptor;
        std::shared_ptr<tcp::socket> socket_ptr;
        bool active_connection;
};

/*
    The TCP client class is a synchronous TCP framework for
    sending/receiving client TCP connections and sending/receiving 
    client packet communications over these connections.
*/
class TCPClient {
    public:
        TCPClient();
        void send_connection(const std::string& addr, uint16_t port);
        packet_t read_packet();
        void write_packet(const packet_t& packet);
        void close_connection();

    private:
        boost::asio::io_context io_context;
        tcp::resolver resolver;
        std::shared_ptr<tcp::socket> socket_ptr;
        bool active_connection;
};

#endif