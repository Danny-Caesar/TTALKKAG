#include <iostream>
#include "socket_broker.h"

// constructor
socket_broker::socket_broker(boost::asio::io_context &io_context)
:
    _acceptor(
        io_context,
        boost::asio::ip::tcp::endpoint(
            // IPv4
            boost::asio::ip::tcp::v4(),
            // broker port
            1883
        )
    ),
    _socket(io_context)
{

}

// destructor
socket_broker::~socket_broker()
{
    
}

// Initialize asynchronous tcp/ip socket acceptions.
void socket_broker::start_accept()
{
    _acceptor.async_accept(
        _socket,
        boost::bind(
            &socket_broker::accept_complete,
            this,
            boost::asio::placeholders::error
        )
    );
}

// Handle connection requests from client.
void socket_broker::accept_complete(
    const boost::system::error_code &ec
)
{
    if(!ec)
    {
        // __log_trace("a new connection was established.");
        std::cout << "a new connection was established.\n";
        read();
    }
    else
    {
        // __log_trace("error occured");
        std::cout << "error occured.\nerror code: " << ec << '\n';
    }
}

// Asynchronously read the socket receive buffer.
void socket_broker::read()
{
    memset(&_receive_buffer, '\0', sizeof(_receive_buffer));
    _socket.async_read_some
    (
        boost::asio::buffer(_receive_buffer),
        boost::bind(
            &socket_broker::read_complete,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

// Handle messages received from client.
void socket_broker::read_complete(
    const boost::system::error_code &ec,
    size_t bytes_received
)
{
    if(ec)
    {
        if(ec == boost::asio::error::eof)
        {
            // __log_trace("client disconnection.\n")
            std::cout << "client disconnection.\n";
        }
        else
        {
            // __log_trace(error.message().c_str());
            std::cout << ec.message().c_str();
        }

        reset();
    }
    else
    {
        auto transactor = [this](
            const void* data,
            const size_t size
        )
        {
            bool send_empty_reply = true;

            if(data && size)
            {

                // receive packet data parsing.
                
            }

            if(send_empty_reply)
            {
                // basic_socket_packet<> reply_packet;
                char* reply_packet;
                write(&reply_packet, sizeof(reply_packet));
            }
        };

        // do transaction.
        transactor(_receive_buffer, bytes_received);
    
        // read next data.
        read();
    }
}

// Write asynchronously in the client socket.
void socket_broker::write(
    const void* data,
    const size_t size
)
{
    boost::asio::async_write(
        _socket,
        boost::asio::buffer(data, size),
        boost::bind(
            &socket_broker::write_complete,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void socket_broker::write_complete(
    const boost::system::error_code&,
    size_t
)
{

}

// Reset the socket.
void socket_broker::reset()
{
    if(_socket.is_open())
    {
        _socket.close();
    }

    start_accept();
}