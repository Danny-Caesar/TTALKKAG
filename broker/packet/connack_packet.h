#pragma once

#include "mqtt_control_packet.h"
#include <vector>

class connack_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        bool session_present = false;
        uint8_t return_code = 0x00;
    } v_header;
public:

    mqtt_packet_type type() const override { return mqtt_packet_type::CONNACK; }
    void handle(socket_broker&) override { /* No need */ }

    static std::unique_ptr<connack_packet> create(bool session_present, uint8_t return_code);

    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;

        // Fixed Header
        packet.push_back(0x20);  // CONNACK (Type = 2 << 4), flags = 0
        packet.push_back(0x02);  // Remaining Length = 2

        // Variable Header
        uint8_t flags = v_header.session_present ? 0x01 : 0x00;
        packet.push_back(flags);        // Connect Acknowledge Flags
        packet.push_back(v_header.return_code);  // Connect Return Code

        return packet;
    }
};