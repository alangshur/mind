#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <utility>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

class EngineServer {
    public:
        EngineServer(uint16_t server_port);
        tcp::socket accept_connection();
        tcp::socket send_connection(const std::string& addr, uint16_t port);
        std::pair<std::string, std::string> read_message(tcp::socket& socket);
        void send_message(tcp::socket& socket, const std::string& protocol,
            const std::string& payload);

    private:
        uint16_t server_port;
        boost::asio::io_service io_service;
};

#endif