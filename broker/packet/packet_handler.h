#pragma once

#include <memory>
#include "mqtt_control_packet.h"
#include "connect_packet.h"
#include "connack_packet.h"
#include "publish_packet.h"
#include "puback_packet.h"
#include "subscribe_packet.h"
#include "suback_packet.h"
#include "unsubscribe_packet.h"
#include "unsuback_packet.h"
#include "pingreq_packet.h"
#include "pingresp_packet.h"
#include "socket_broker.h"


class packet_handler
{
public:
    // static std::vector<uint8_t> handle(const uint8_t* data, size_t size, std::unique_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle(const uint8_t* data, size_t size, std::shared_ptr<socket_broker> socket);

private:
    static std::vector<uint8_t> handle_connect(connect_packet& packet, std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_publish(publish_packet& packet, std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_puback(puback_packet& packet, std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_subscribe(subscribe_packet& packet, std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_unsubscribe(unsubscribe_packet& packet, std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_pingreq(std::shared_ptr<socket_broker> socket);
    static std::vector<uint8_t> handle_disconnect(std::shared_ptr<socket_broker> socket);
};
