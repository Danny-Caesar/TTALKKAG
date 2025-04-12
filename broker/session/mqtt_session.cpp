#include "mqtt_session.h"

mqtt_session::mqtt_session(connect_packet packet, socket_broker* socket)
{
    client_connect = true;
    clean_session = packet.v_header.connect_flags.clean_session;
    socket->set_client_id(packet.client_id);
    this->socket = socket;
    client_id = packet.client_id;
    user_name = packet.user_name;
    password = packet.password;
    will_topic = packet.will_topic;
    will_message = packet.will_message;
    will_qos = packet.v_header.connect_flags.will_qos;
    keep_alive = packet.v_header.keep_alive;
}

// Open session when client reconnected.
// Values in new connect packet would be ignored.
void mqtt_session::open_session(socket_broker* socket)
{
    client_connect = true;
    this->socket = socket;
}

// Close session when client disconnected.
void mqtt_session::close_session()
{
    client_connect = false;
    socket->close();
}

void mqtt_session::debug()
{
    std::cout << "----Session----\n";
    std::cout << "client_id: " << client_id << '\n';
    std::cout << "user_name: " << user_name << '\n';
    std::cout << "password: " << password << '\n';
    std::cout << "clean_session: " << clean_session << '\n';
    std::cout << "will_topic: " << will_topic << '\n';
    std::cout << "will_message: " << will_message << '\n';
    std::cout << "will_qos: " << will_qos << '\n';
    std::cout << "keep_alive: " << keep_alive << "\n\n";
}

//
// legacy
//

// // Set session fields based on connect packet.
// mqtt_session mqtt_session::operator=(const mqtt_session& session)
// {
//     client_connect = session.client_connect;
//     clean_session = session.clean_session;
//     this->socket = session.socket;
//     client_id = session.client_id;
//     user_name = session.user_name;
//     password = session.password;
//     will_topic = session.will_topic;
//     will_message = session.will_message;
//     will_qos = session.will_qos;
//     keep_alive = session.keep_alive;
// }