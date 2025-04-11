#include <string>
#include <queue>
#include "socket_broker.h"
#include "connect_packet.h"

struct message
{
    std::string topic;
    std::string message;
};

class mqtt_session
{
public:
    bool client_connect;
    bool clean_session;
    socket_broker* socket;

    std::string client_id;
    std::string user_name;
    std::string password;

    uint8_t will_qos;
    std::string will_topic;
    std::string will_message;

    uint16_t keep_alive;

public:
    mqtt_session(connect_packet packet, socket_broker* socket);

    void set_session(connect_packet packet, socket_broker* socket);
    void open_session(socket_broker* socket);
    void close_session();

private:
    std::queue<message> _message_queue;
};