#include <iostream>
#include <boost/asio.hpp>
#include "socket_broker.h"
#include "subscription/subscription_manager.h"

int main()
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::acceptor acceptor(
        io_context,
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(),
            atoi(std::getenv("MQTT_PORT")))
    );

    std::function<void()> start_accept = [&]()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

        acceptor.async_accept(*socket, [&, socket](const boost::system::error_code& ec)
        {
            if (!ec)
            {
                std::cout << "a new connection was established.\n";
                std::shared_ptr<socket_broker> broker = std::make_shared<socket_broker>(std::move(*socket));
                broker->start();  // Start read loop
            }

            start_accept();  // Keep accepting more clients
        });
    };

    start_accept();
    io_context.run();

    return 0;
}
