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

packet_t TCPServer::build_packet(const std::string& protocol_str, 
    const std::string& payload_str) {
    try {
        assert(protocol_str.size() <= PROTOCOL_BYTES);
        assert(payload_str.size() <= PAYLOAD_BYTES);
        return {
            protocol_str + std::string(PROTOCOL_BYTES - protocol_str.size(), '-'), 
            payload_str + std::string(PAYLOAD_BYTES - payload_str.size(), '-')
        };
    }
    catch (std::exception& e) {
        this->error_flag = true;
        return {};
    }
}

void TCPServer::write_packet(const packet_t& packet) {
    try {
        boost::asio::write(
            *(this->socket_ptr), 
            boost::asio::buffer(packet.protocol + packet.payload)
        );
    }
    catch (std::exception& e) {
        this->error_flag = true;
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