#pragma once

#include <stdint.h>
#include "mqtt_control_packet.h"

struct fixed_header {
    // MQTT packet type
    mqtt_packet_type packet_type;
    // Flags according control packet type
    uint8_t flags;
    // Length of packet except fixed header
    uint32_t remaining_length;
    // Length of fixed header self
    size_t header_length;

    // Parse byte stream to a fixed header format.
    static fixed_header parse(const uint8_t* data, size_t size);
    // Read remaining length from byte stream.
    std::pair<uint32_t, size_t> decode_remaining_length(const uint8_t* data, size_t size);
    // Print fields of fixed header.
    void print_fixed_header();
};