#include "util/tcp-server.hpp"
using boost::asio::ip::tcp;

void TPCConnection::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
    [this, self](std::error_code ec, std::size_t length) {
        if (!ec) { do_write(length); }
    });
}

void TPCConnection::do_write(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
    [this, self](std::error_code ec, std::size_t) {
        if (!ec) { do_read(); }
    });
}

TCPServer::TCPServer(boost::asio::io_context& io_context, uint16_t port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), 
    socket_(io_context), io_context(io_context) { do_accept(); }

void TCPServer::do_accept() {
    acceptor_.async_accept(
        socket_,
        [this](std::error_code ec) {
            if (!ec) { std::make_shared<TPCConnection>(std::move(socket_))->start(); }
            do_accept();
        }
    );
}