#pragma once

#include <unistd.h>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "publish_packet.h"
#include "puback_packet.h"

class socket_broker : public std::enable_shared_from_this<socket_broker>
{
public:
    // 8KB
    static const size_t buffer_size = 8192;

public:
    socket_broker(boost::asio::ip::tcp::socket socket);
    ~socket_broker();
    
    void start_accept();
    void start();
    void close();

    void send_packet(const publish_packet& packet);
    void send_packet(const puback_packet& packet);

    void set_client_id(std::string client_id);
    std::string get_client_id();
    
    boost::asio::ip::tcp::socket& native_object() { return _socket; }
    
    // socket_broker(boost::asio::io_context io_context);
    
private:
    void read();
    void write(const void* data, const size_t size);
    void read_complete(const boost::system::error_code& error, size_t bytes_received);
    void write_complete(const boost::system::error_code&, size_t);
    
    // void accept_complete(const boost::system::error_code& error);
    
private:
    std::string _client_id;
    uint8_t _receive_buffer[buffer_size];
    boost::asio::ip::tcp::socket _socket;
    // int _sequence_number;
    // boost::asio::ip::tcp::acceptor _acceptor;
};