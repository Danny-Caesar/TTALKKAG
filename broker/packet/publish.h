#pragma once

#include "mqtt_control_packet.h"
#include <string>

class publish : public mqtt_control_packet
{
public:
    struct variable_header
    {
        std::string topic_name;
        uint16_t packet_identifier;

        static variable_header parse(const uint8_t* data, size_t size);
        void debug();
    } v_header;
    std::string message;
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::PUBLISH; }
    void handle(socket_broker& broker) override;

    static std::unique_ptr<publish> parse(const uint8_t* data, size_t size);
};
