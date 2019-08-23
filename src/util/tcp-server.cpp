#include "util/tcp-server.hpp"

using namespace boost::asio;
using ip::tcp;

EngineServer::EngineServer(uint16_t port) : server_port(server_port),
    io_service(boost::asio::io_service()) {}

tcp::socket EngineServer::accept_connection() {
    tcp::acceptor acceptor(this->io_service, tcp::endpoint(tcp::v4(), 
        this->server_port));
    tcp::socket socket(this->io_service);
    acceptor.accept(socket);
    return socket;
}

tcp::socket EngineServer::send_connection(const std::string& addr, uint16_t port) {
    tcp::socket socket(this->io_service);
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(addr), port));
    return socket;
}

std::pair<std::string, std::string> EngineServer::read_message(tcp::socket& socket) {
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