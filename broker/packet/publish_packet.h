#pragma once

#include "mqtt_control_packet.h"
#include "fixed_header.h"
#include <string>

class publish_packet : public mqtt_control_packet
{
public:
    uint8_t dup;
    uint8_t qos;
    uint8_t retain;
    struct variable_header
    {
        std::string topic_name;
        uint16_t packet_identifier;

        static variable_header parse(const uint8_t* data, size_t size, uint8_t qos);
        void debug();
    } v_header;
    std::string message;

public:
    mqtt_packet_type type() const override { return mqtt_packet_type::PUBLISH; }
    static std::unique_ptr<publish_packet> parse(const uint8_t* data, size_t size, uint8_t flags);

    void debug();

    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;
        
        // 1. Fixed header
        // PUBLISH (3 << 4), DUP, QoS, Retain
        packet.push_back(0x30 | (dup << 3) | (qos << 1) | retain);
        // Remaining length
        uint16_t topic_len = v_header.topic_name.length();
        uint32_t remaining_len = 2 + topic_len + message.length();
        if(qos > 0) remaining_len += 2;
        std::vector<uint8_t> remaining_len_bytes = fixed_header::encode_remaining_length(remaining_len);
        packet.insert(packet.end(), remaining_len_bytes.begin(), remaining_len_bytes.end());

        // 2. Variable header
        // Topic name length
        packet.push_back((topic_len >> 8) & 0xFF);
        packet.push_back(topic_len & 0xFF);
        // Topic name
        for(size_t i = 0; i < topic_len; i++)
        {
            packet.push_back(v_header.topic_name[i]);
        }
        // Packet identifier
        if(qos > 0)
        {
            uint16_t packet_id = v_header.packet_identifier;
            packet.push_back((packet_id >> 8) & 0xFF);
            packet.push_back(packet_id & 0xFF);
        }

        // 3. Payload
        // Application message
        for(size_t i = 0; i < message.length(); i++)
        {
            packet.push_back(message[i]);
        }

        return packet;
    };
};
