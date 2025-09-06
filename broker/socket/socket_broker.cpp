#include <iostream>
#include "socket_broker.h"
#include "mqtt_control_packet.h"
#include "packet_handler.h"
#include "packet_counter.h"

// Constructor
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

std::string socket_broker::get_client_id()
{
    return _client_id;
}

void socket_broker::start()
{
    read();
}

void socket_broker::send_packet(const publish_packet& packet)
{
    // Do not transmit if socket is closed
    if(!_socket.is_open()) return;

    std::vector<uint8_t> bytes = packet.serialize();
    write(bytes.data(), bytes.size());
}

void socket_broker::send_packet(const puback_packet& packet)
{
    // Do not transmit if socket is closed
    if(!_socket.is_open()) return;

    std::vector<uint8_t> bytes = packet.serialize();
    write(bytes.data(), bytes.size());
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
            std::cout << "socket(" << _client_id << "): " << ec.message().c_str() << '\n';
        
        close();
    }
    else
    {
        PacketCounter::getInstance().add(bytes_received);
        auto transactor = [this](
            const uint8_t* data,
            const size_t size
        )
        {
            if(data && size)
            {
                // Receive packet data parsing.
                std::vector<uint8_t> reply_packet = packet_handler::handle(data, size, shared_from_this());
                
                if(!reply_packet.empty()) write(reply_packet.data(), reply_packet.size());
            }
        };

        // Do transaction.
        transactor(_receive_buffer, bytes_received);
    
        // Stop if client disconnected.
        if(!_socket.is_open()) return;

        // Read next data.
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
            shared_from_this(),
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

// // Handle connection requests from client.
// void socket_broker::accept_complete(const boost::system::error_code &ec)
// {
//     if(!ec)
//     {
//         std::cout << "New connection established.\n";
//         read();
//     }
//     else
//     {
//         std::cout << "Error occured.\nerror code: " << ec << '\n';
//     }
// }