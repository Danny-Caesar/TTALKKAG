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

        static variable_header parse(const uint8_t* data, size_t size);
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
        packet.push_back(0x20 | (dup << 3) | (qos << 1) | retain); // PUBLISH (2 << 4), DUP, QoS, Retain

        // 2. Variable header
        // Topic name length
        uint16_t topic_len = v_header.topic_name.length();
        packet.push_back((topic_len >> 8) & 0xFF);
        packet.push_back(topic_len & 0xFF);
        // Topic name
        for(size_t i = 0; i < topic_len; i++)
        {
            packet.push_back(v_header.topic_name[i]);
        }
        // Packet identifier
        uint16_t packet_id = v_header.packet_identifier;
        packet.push_back((packet_id >> 8) & 0xFF);
        packet.push_back(packet_id & 0xFF);

        // 3. Payload
        // Application message
        for(size_t i = 0; i < message.length(); i++)
        {
            packet.push_back(message[i]);
        }
    };
};
