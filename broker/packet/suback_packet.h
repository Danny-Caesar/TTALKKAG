#pragma once

#include <vector>
#include "mqtt_control_packet.h"
#include "fixed_header.h"

class suback_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        uint16_t packet_identifier;

        void debug();
    } v_header;
    std::vector<uint8_t> return_code;

public:
    mqtt_packet_type type() const override { return mqtt_packet_type::SUBACK; }
    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;

        // 1. Fixed header
        packet.push_back(0x90); // SUBACK (Type = 9 << 4), Flags = 0
        uint32_t remaining_length = 2 + return_code.size();
        std::vector<uint8_t> remaining_length_bytes = fixed_header::encode_remaining_length(remaining_length);
        packet.insert(packet.end(), remaining_length_bytes.begin(), remaining_length_bytes.end());

        // 2. Variable header
        // Packet idetifier
        packet.push_back((v_header.packet_identifier >> 8) & 0xFF);
        packet.push_back(v_header.packet_identifier & 0xFF);

        // 3. Payload
        // Return code
        for(uint8_t rc : return_code)
            packet.push_back(rc);

        return packet;
    }
    void debug();
    
    static std::unique_ptr<suback_packet> parse(const uint8_t* data, size_t size);
    static std::unique_ptr<suback_packet> create(uint16_t packet_identifier, std::vector<uint8_t> return_code);
};