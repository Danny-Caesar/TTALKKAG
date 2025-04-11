#pragma once

#include <memory>
#include "mqtt_control_packet.h"
#include "connect_packet.h"
#include "connack_packet.h"
#include "publish_packet.h"
#include "subscribe_packet.h"
#include "suback_packet.h"
#include "socket_broker.h"


class packet_handler {
public:
    static std::vector<uint8_t> handle(const uint8_t* data, size_t size, socket_broker* socket);

private:
    static std::vector<uint8_t> handle_connect(connect_packet& packet, socket_broker* socket);
    static std::vector<uint8_t> handle_publish(publish_packet& packet);
    static std::vector<uint8_t> handle_subscribe(subscribe_packet& packet, socket_broker* socket);
    static std::vector<uint8_t> handle_disconnect(socket_broker* socket);
};
