#include "util/tcp-server.hpp"

using namespace boost::asio;
using ip::tcp;

EngineServer::EngineServer(uint16_t port) : port(port) {}

tcp::socket EngineServer::accept_connection() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), this->port));
    tcp::socket socket(io_service);
    acceptor.accept(socket);
    return socket;
}

std::pair<std::string, std::string> EngineServer::read_message(tcp::socket & socket) {
    boost::asio::streambuf protocol_buf, payload_buf;
    boost::asio::read_until(socket, protocol_buf, "\n");
    boost::asio::read_until(socket, payload_buf, "\n");
    return std::pair<std::string, std::string>(
        boost::asio::buffer_cast<const char*>(protocol_buf.data()),
        boost::asio::buffer_cast<const char*>(payload_buf.data())
    );
}

void send_message(tcp::socket& socket, const std::string& protocol, 
    const std::string& payload) {
    const std::string msg = protocol + "\n" + payload + "\n";
    boost::asio::write(socket, boost::asio::buffer(msg));
}