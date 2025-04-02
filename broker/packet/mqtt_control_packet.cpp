#include <stdexcept>
#include <iostream>
#include "mqtt_control_packet.h"
#include "fixed_header.h"
#include "connect_packet.h"
#include "connack_packet.h"
#include "publish_packet.h"

// Parse byte stream to a packet format.
std::unique_ptr<mqtt_control_packet> mqtt_control_packet::mqtt_control_packet::parse(const uint8_t* data, size_t size)
{
    // Throw runtime error if size is too small.
    if (size < 2) throw std::runtime_error("Packet too small");

    // Parse fixed header.
    fixed_header header = fixed_header::parse(data, size);
    
    // Debug fixed hedaer.
    header.debug();

    // Payload pointer. (or variable header pointer to some packet types.)
    const uint8_t* payload = data + header.header_length;

    switch (header.packet_type)
    {
        case mqtt_packet_type::CONNECT:
            return connect_packet::parse(payload, header.remaining_length);
        case mqtt_packet_type::CONNACK:
            return NULL;
        case mqtt_packet_type::PUBLISH:
            return publish_packet::parse(payload, header.remaining_length);
        // Other parse functions...
        default:
            throw std::runtime_error("Unsupported packet type");
    }
}