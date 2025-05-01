#include "mqtt_session.h"
#include "../subscription/subscription_manager.h"

// Initialization
std::unordered_map<std::string, publish_packet> mqtt_session::_message_map;

mqtt_session::mqtt_session(connect_packet& packet, std::shared_ptr<socket_broker> socket)
{
    client_connect = true;
    clean_session = packet.v_header.connect_flags.clean_session;
    socket->set_client_id(packet.client_id);
    this->socket = std::move(socket);
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
void mqtt_session::open_session(std::shared_ptr<socket_broker> socket)
{
    client_connect = true;
    this->socket = std::move(socket);
}

// Close session when client disconnected.
void mqtt_session::close_session()
{
    client_connect = false;
    socket->close();
}

// Handle message according retain flag
void mqtt_session::retain_message(publish_packet& packet)
{
    std::string topic_name = packet.v_header.topic_name;

    if(packet.retain)
    {
        if(packet.message.size() == 0){
            // Discard current message.
            // Discard message retained.
            _message_map.erase(topic_name);
            return;
        }

        // Set retain.
        packet.retain = 0;

        if(packet.qos > 0)
        {
            // Overwrite retained message.
            _message_map[topic_name] = packet;
        }
        else
        {
            // Discard retained message.
            _message_map[topic_name] = packet;
        }
    }

    // Nothing happens when retain is 0.
}

// Send all messages retained while client was offline.
void mqtt_session::flush_message()
{
    if(!client_connect) return;

    subscription_manager& sub_mgr = subscription_manager::get_instance();

    for(auto it : _message_map)
    {
        std::string topic_name = it.first;
        publish_packet message = it.second;

        // Decide QoS.
        subscription sub = *sub_mgr.get_subscription(topic_name, client_id);
        message.qos = std::min(message.qos, sub.qos);

        // Unset retain.
        message.retain = 0;

        socket->send_packet(it.second);
    }

    _message_map.clear();
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