#pragma once

#include <memory>
#include <string>
#include "mqtt_packet_types.h"

// MQTT base packet
class mqtt_control_packet
{
    public:
        virtual ~mqtt_control_packet() = default;
        // Control packet type
        virtual mqtt_packet_type type() const = 0;

        // Parse byte stream to a packet format.
        static std::unique_ptr<mqtt_control_packet> parse(const uint8_t* data, size_t size);
};

class connect_packet : public mqtt_control_packet
{
    public:
        std::string client_id;
        bool clean_session;
    
        static std::unique_ptr<connect_packet> parse(const uint8_t* payload, size_t length);
    
        mqtt_packet_type type() const override { return mqtt_packet_type::CONNECT; }
};