#include "socket/socket_broker.h"

int main(void)
{
    boost::asio::io_context io_context;

    socket_broker socket(io_context);
    socket.start_accept();

    io_context.run();

    return 0;
}