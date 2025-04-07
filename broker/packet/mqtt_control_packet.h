#pragma once

#include <iostream>
#include <stdexcept>
#include <memory>
#include "mqtt_packet_types.h"

class socket_broker; // forward declaration

class mqtt_control_packet
{
public:
    virtual ~mqtt_control_packet() = default;
    virtual mqtt_packet_type type() const = 0;

    static std::unique_ptr<mqtt_control_packet> parse(const uint8_t* data, size_t size);
};