#pragma once

#include <vector>
#include "mqtt_control_packet.h"
#include "fixed_header.h"

class unsuback_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        uint16_t packet_identifier;

        void debug();
    } v_header;

public:
    mqtt_packet_type type() const override { return mqtt_packet_type::UNSUBACK; }
    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;

        // 1. Fixed header
        packet.push_back(0xB0); // UNSUBACK (Type = 11 << 4), Flags = 0
        uint32_t remaining_length = 2;
        std::vector<uint8_t> remaining_length_bytes = fixed_header::encode_remaining_length(remaining_length);
        packet.insert(packet.end(), remaining_length_bytes.begin(), remaining_length_bytes.end());

        // 2. Variable header
        // Packet idetifier
        packet.push_back((v_header.packet_identifier >> 8) & 0xFF);
        packet.push_back(v_header.packet_identifier & 0xFF);

        return packet;
    }
    void debug();

    static std::unique_ptr<unsuback_packet> create(uint16_t packet_identifier);
};