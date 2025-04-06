#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "mqtt_control_packet.h"

class subscribe_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        uint16_t packet_identifier;

        static variable_header parse(const uint8_t* data, size_t size);
        void debug();
    } v_header;

    std::vector<std::string> topic_filter;
    std::vector<uint8_t> qos_request;
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::SUBSCRIBE; };
    void handle(socket_broker& broker) override;

    static std::unique_ptr<subscribe_packet> parse(const uint8_t* data, size_t size);

    static std::unique_ptr<subscribe_packet> create(
        uint16_t packet_identifier,
        std::vector<std::string> topic_filter,
        std::vector<uint8_t> qos_request
    );

    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;

        // 1. Fixed header
        packet.push_back(0x82);  // CONNACK (Type = 8 << 4), flags = 2
        uint8_t remaining_length = 2 + 2;
        for(size_t i = 0; i < topic_filter.size(); i++) { remaining_length += (topic_filter[i].length() + 1); }
        packet.push_back(remaining_length);

        // 2. Variable header
        packet.push_back((v_header.packet_identifier >> 8) & 0xFF);
        packet.push_back(v_header.packet_identifier & 0xFF);

        // 3. Payload
        for(size_t i = 0; i < topic_filter.size(); i++)
        {
            std::string tf = topic_filter[i];
            // Length
            packet.push_back((tf.length() >> 8) & 0xFF);
            packet.push_back(tf.length() & 0xFF);
            // Topic filter
            size_t length_bytes = round((float)tf.length() / 8);
            for(size_t j = 0; i < length_bytes; i++) { packet.push_back(tf[j]); }
            // Requested QoS
            packet.push_back(qos_request[i]);
        }

        return packet;
    }

    void debug();
};