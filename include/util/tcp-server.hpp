#ifndef TCPTCPServer_H
#define TCPTCPServer_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
using boost::asio::ip::tcp;

class TPCConnection: public std::enable_shared_from_this <TPCConnection> {
    public: 
        TPCConnection(tcp::socket socket): socket_(std::move(socket)) {}
        void start() { do_read(); }

    private: 
        void do_read();
        void do_write(std::size_t length);

        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
};

class TCPServer {
    public:
        TCPServer(boost::asio::io_context& io_context, uint16_t port);
        void run() { this->io_context.run(); }
  
    private:
        void do_accept();

        tcp::acceptor acceptor_;
        tcp::socket socket_;
        boost::asio::io_context& io_context;
};

#endif