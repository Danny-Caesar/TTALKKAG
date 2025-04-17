#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "mqtt_control_packet.h"

class unsubscribe_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        uint16_t packet_identifier;

        static variable_header parse(const uint8_t* data, size_t size);
        void debug();
    } v_header;

    std::vector<std::string> topic_filter;
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::UNSUBSCRIBE; };

    static std::unique_ptr<unsubscribe_packet> parse(const uint8_t* data, size_t size);

    void debug();
};