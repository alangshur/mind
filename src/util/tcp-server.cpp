#include "util/tcp-server.hpp"

TCPServer::TCPServer(uint16_t port) : io_context(), acceptor(this->io_context, 
    tcp::endpoint(tcp::v4(), port)), socket_ptr(nullptr) {}

void TCPServer::accept_connection() {
    try {
        this->socket_ptr = std::make_shared<tcp::socket>(tcp::socket(this->io_context));
        this->acceptor.accept(*(this->socket_ptr));
        this->active_connection = true;
    }
    catch (std::exception& e) {
        this->error_flag = true;
    }
}

packet_t TCPServer::read_packet() {
    try {
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
    catch (std::exception& e) {
        this->error_flag = true;
        return {};
    }
}

void TCPServer::close_connection() {
    try {
        (*(this->socket_ptr)).close();
        this->active_connection = false;
    }
    catch (std::exception& e) {
        this->error_flag = true;
    }
}