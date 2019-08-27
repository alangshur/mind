#include "util/tcp-server.hpp"

TCPServer::TCPServer(uint16_t port) : io_context(), acceptor(this->io_context, 
    tcp::endpoint(tcp::v4(), port)), socket_ptr(nullptr), active_connection(false) {}

void TCPServer::accept_connection() {
    if (this->active_connection) throw std::runtime_error("Connection accept failed since "
        "active connection already exists (server).");
    this->socket_ptr = std::make_shared<tcp::socket>(tcp::socket(this->io_context));
    this->acceptor.accept(*(this->socket_ptr));
    this->active_connection = true;
}

packet_t TCPServer::read_packet() {
    if (!this->active_connection) throw std::runtime_error("Packet read failed since "
        "there is no active connection (server).");
    boost::asio::basic_streambuf packet_buf;
    boost::asio::read(*(this->socket_ptr), packet_buf, 
        boost::asio::transfer_exactly(PACKET_BYTES));
    boost::asio::streambuf::const_buffers_type buf = packet_buf.data();
    return {
        std::string(
            boost::asio::buffers_begin(buf),
            boost::asio::buffers_begin(buf) + PROTOCOL_BYTES
        ),
        std::string(
            boost::asio::buffers_begin(buf) + PROTOCOL_BYTES,
            boost::asio::buffers_begin(buf) + PACKET_BYTES
        )
    };
}

void TCPServer::write_packet(const packet_t& packet) {
    if (!this->active_connection) throw std::runtime_error("Packet write failed since "
        "there is no active connection (server).");
    boost::asio::write(
        *(this->socket_ptr), 
        boost::asio::buffer(packet.protocol + packet.payload)
    );
} 

void TCPServer::close_connection() {
    if (!this->active_connection) throw std::runtime_error("Connection close failed since "
        "there is no active connection (server).");
    (*(this->socket_ptr)).close();
    this->active_connection = false;
}

TCPClient::TCPClient() : io_context(), resolver(this->io_context),
    socket_ptr(nullptr), active_connection(false) {}

void TCPClient::send_connection(const std::string& addr, uint16_t port) {
    if (this->active_connection) throw std::runtime_error("Connection send failed since "
        "active connection already exists (client).");
    this->socket_ptr = std::make_shared<tcp::socket>(tcp::socket(this->io_context));
    tcp::resolver::query query(addr, std::to_string(port));
    tcp::resolver::results_type endpoint = this->resolver.resolve(query);
    boost::asio::connect(*(this->socket_ptr), endpoint);
    this->active_connection = true;
}

packet_t TCPClient::read_packet() {
    if (!this->active_connection) throw std::runtime_error("Packet read failed since "
        "there is no active connection (client).");
    boost::asio::basic_streambuf packet_buf;
    boost::asio::read(*(this->socket_ptr), packet_buf, 
        boost::asio::transfer_exactly(PACKET_BYTES));
    boost::asio::streambuf::const_buffers_type buf = packet_buf.data();
    return {
        std::string(
            boost::asio::buffers_begin(buf),
            boost::asio::buffers_begin(buf) + PROTOCOL_BYTES
        ),
        std::string(
            boost::asio::buffers_begin(buf) + PROTOCOL_BYTES,
            boost::asio::buffers_begin(buf) + PACKET_BYTES
        )
    };
}

void TCPClient::write_packet(const packet_t& packet) {
    if (!this->active_connection) throw std::runtime_error("Packet write failed since "
        "there is no active connection (client).");
    boost::asio::write(
        *(this->socket_ptr), 
        boost::asio::buffer(packet.protocol + packet.payload)
    );
} 

void TCPClient::close_connection() {
    if (!this->active_connection) throw std::runtime_error("Connection close failed since "
        "there is no active connection (client).");
    (*(this->socket_ptr)).close();
    this->active_connection = false;
}