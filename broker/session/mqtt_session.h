#pragma once

#include <string>
#include <queue>
#include "socket_broker.h"
#include "connect_packet.h"
#include "publish_packet.h"

class mqtt_session
{
public:
    std::shared_ptr<socket_broker> socket;
    bool client_connect;
    bool clean_session;

    std::string client_id;
    std::string user_name;
    std::string password;

    std::string will_topic;
    std::string will_message;
    uint8_t will_qos;

    uint16_t keep_alive;

public:
    mqtt_session() = default;
    mqtt_session(connect_packet& packet, std::shared_ptr<socket_broker> socket);
    // mqtt_session operator=(const mqtt_session& session);
    // mqtt_session(const mqtt_session& session);

    void open_session(std::shared_ptr<socket_broker> socket);
    void close_session();

    void retain_message(publish_packet& packet);
    void flush_message();

    void debug();

private:
    static std::unordered_map<std::string, publish_packet> _message_map;
};