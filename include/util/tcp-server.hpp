#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

/*
    The generic TCP server class is a synchronous TCP framework for
    sending/receiving server TCP connections and sending/receiving 
    server packet communications over these connections.
*/
template <typename T>
class TCPServer {
    public:
        TCPServer(uint16_t port);
        void accept_connection();
        T read_packet();
        void write_packet(const T& ds_packet);
        void close_connection();

    private:
        boost::asio::io_context io_context;
        tcp::acceptor acceptor;
        std::shared_ptr<tcp::socket> socket_ptr;
        bool active_connection;
};

template <typename T>
TCPServer<T>::TCPServer(uint16_t port) : io_context(), acceptor(this->io_context, 
    tcp::endpoint(tcp::v4(), port)), socket_ptr(nullptr), active_connection(false) {}

template <typename T>
void TCPServer<T>::accept_connection() {
    if (this->active_connection) throw std::runtime_error("Connection accept failed since "
        "active connection already exists.");

    // accept queued connection
    this->socket_ptr = std::make_shared<tcp::socket>(tcp::socket(this->io_context));
    this->acceptor.accept(*(this->socket_ptr));
    this->active_connection = true;
}

#include <iostream>
template <typename T>
T TCPServer<T>::read_packet() {
    if (!this->active_connection) throw std::runtime_error("Packet read failed since "
        "there is no active connection.");

    // read generic packet T
    boost::asio::streambuf read_buf;
    boost::asio::read(*(this->socket_ptr), 
        read_buf, boost::asio::transfer_exactly(sizeof(T)));
    std::string s_packet((std::istreambuf_iterator<char>(&read_buf)), 
        std::istreambuf_iterator<char>());

    // deserialize packet
    T ds_packet;
    memcpy(&ds_packet, s_packet.c_str(), sizeof(T));
    return ds_packet;
}

template <typename T>
void TCPServer<T>::write_packet(const T& ds_packet) {
    if (!this->active_connection) throw std::runtime_error("Packet write failed since "
        "there is no active connection.");

    // serialize packet
    char s_packet[sizeof(T)];
    memcpy(&s_packet[0], &ds_packet, sizeof(T));

    // write generic packet T
    boost::asio::write(
        *(this->socket_ptr), 
        boost::asio::buffer(s_packet, sizeof(T))
    );
} 

template <typename T>
void TCPServer<T>::close_connection() {
    if (!this->active_connection) throw std::runtime_error("Connection close failed since "
        "there is no active connection.");

    // close active socket
    (*(this->socket_ptr)).close();
    this->active_connection = false;
}

#endif