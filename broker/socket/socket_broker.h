#pragma once

#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


class socket_broker : public std::enable_shared_from_this<socket_broker>
{
    public:
        // 8KB
        static const size_t buffer_size = 8192;

    public:
        // socket_broker(boost::asio::io_context io_context);
        socket_broker(boost::asio::ip::tcp::socket socket);
        ~socket_broker();

        boost::asio::ip::tcp::socket& native_object()
        {
            return _socket;
        }
        void start(); // Start reading packets
        void start_accept();
    
    private:
        void read();
        void write(const void* data, const size_t size);
        void read_complete(const boost::system::error_code& error, size_t bytes_received);
        void write_complete(const boost::system::error_code&, size_t);
        void accept_complete(const boost::system::error_code& error);
        void reset();
    
    private:
        // int _sequence_number;
        uint8_t _receive_buffer[buffer_size];

        // boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::ip::tcp::socket _socket;
};