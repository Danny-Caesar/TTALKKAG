#pragma once

#include "mqtt_control_packet.h"
#include <string>

class connect_packet : public mqtt_control_packet
{
public:
    std::string client_id;
    bool clean_session;

    mqtt_packet_type type() const override { return mqtt_packet_type::CONNECT; }
    void handle(socket_broker& broker) override;

    static std::unique_ptr<connect_packet> parse(const uint8_t* data, size_t size);
};
