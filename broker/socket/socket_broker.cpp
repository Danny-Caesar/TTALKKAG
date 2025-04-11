#include <iostream>
#include "socket_broker.h"
#include "mqtt_control_packet.h"
#include "packet_handler.h"

// constructor
socket_broker::socket_broker(boost::asio::ip::tcp::socket socket)
: _socket(std::move(socket))
{

}

socket_broker::~socket_broker()
{
    
}

void socket_broker::set_client_id(std::string client_id)
{
    _client_id = client_id;
}

std::string socket_broker::get_clinet_id()
{
    return _client_id;
}

void socket_broker::start()
{
    read();
}

// Handle connection requests from client.
void socket_broker::accept_complete(const boost::system::error_code &ec)
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

// Asynchronously read receive buffer of socket.
void socket_broker::read()
{
    memset(&_receive_buffer, 0, sizeof(_receive_buffer));
    _socket.async_read_some(
        boost::asio::buffer(_receive_buffer),
        boost::bind(
            &socket_broker::read_complete,
            shared_from_this(),
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
            std::cout << "Client disconnected.\n";
        else
            std::cout << ec.message().c_str();

        close();
    }
    else
    {
        auto transactor = [this](
            const uint8_t* data,
            const size_t size
        )
        {
            if(data && size)
            {
                // receive packet data parsing.
                std::vector<uint8_t> reply_packet = packet_handler::handle(data, size, this);
                
                if(!reply_packet.empty()) write(reply_packet.data(), reply_packet.size());
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

// Close the socket.
void socket_broker::close()
{
    if(_socket.is_open())
    {
        _socket.close();
    }
}


//
// legacy
//

// socket_broker::socket_broker(boost::asio::io_context io_context)
// : _acceptor(
//         io_context,
//         boost::asio::ip::tcp::endpoint(
//             // IPv4
//             boost::asio::ip::tcp::v4(),
//             // MQTT port
//             atoi(std::getenv("MQTT_PORT"))
//         )
//     ),
//     _socket(io_context)
// {

// }

// Initialize asynchronous tcp/ip socket acceptions.
// void socket_broker::start_accept()
// {
//     _acceptor.async_accept(
//         _socket,
//         boost::bind(
//             &socket_broker::accept_complete,
//             this,
//             boost::asio::placeholders::error
//         )
//     );
// }