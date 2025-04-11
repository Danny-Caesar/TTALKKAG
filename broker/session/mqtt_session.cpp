#include "mqtt_session.h"

mqtt_session::mqtt_session(connect_packet packet, socket_broker* socket)
{
    client_connect = true;
    clean_session = packet.v_header.connect_flags.clean_session;
    this->socket = socket;
    client_id = packet.client_id;
    user_name = packet.user_name;
    password = packet.password;
    will_topic = packet.will_topic;
    will_message = packet.will_message;
    will_qos = packet.v_header.connect_flags.will_qos;
    keep_alive = packet.v_header.keep_alive;
}

// Set session fields based on connect packet.
void mqtt_session::set_session(connect_packet packet, socket_broker* socket)
{
    client_connect = true;
    clean_session = packet.v_header.connect_flags.clean_session;
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
    // socket->close();
}