#pragma once

#include <stdint.h>
#include "mqtt_control_packet.h"

struct fixed_header
{
    mqtt_packet_type packet_type;   // Flags according control packet type
    uint8_t flags;
    uint32_t remaining_length;      // Length of packet except fixed header
    size_t header_length;

    static fixed_header parse(const uint8_t* data, size_t size);
    std::pair<uint32_t, size_t> decode_remaining_length(const uint8_t* data, size_t size);  // Read remaining length from byte stream.
    void debug();
};