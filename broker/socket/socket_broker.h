#pragma once

#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


class socket_broker
{
    public:
        // 8KB
        static const size_t buffer_size = 8192;

    public:
        socket_broker(boost::asio::io_context &io_context);
        ~socket_broker();

        boost::asio::ip::tcp::socket& native_object()
        {
            return _socket;
        }

        void start_accept();
        
        void write(
            const void* data,
            const size_t size
        );
    
    private:
        void read_complete(
            const boost::system::error_code& error,
            size_t bytes_received
        );


        void write_complete(
            const boost::system::error_code&,
            size_t
        );

        void reset();

        void read();

        void accept_complete(const boost::system::error_code& error);
    
    private:
        // int _sequence_number;
        uint8_t _receive_buffer[buffer_size];

        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::ip::tcp::socket _socket;
};