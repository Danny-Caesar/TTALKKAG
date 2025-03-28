#include <stdexcept>
#include <iostream>
#include "socket/socket_broker.h"
#include "mqtt_control_packet.h"
#include "fixed_header.h"

// Parse byte stream to a packet format.
std::unique_ptr<mqtt_control_packet> mqtt_control_packet::mqtt_control_packet::parse(const uint8_t* data, size_t size)
{
    // Throw runtime error if size is too small.
    if (size < 2) throw std::runtime_error("Packet too small");

    // Parse fixed header.
    fixed_header header = fixed_header::parse(data, size);

    // Pointing payload
    const uint8_t* payload = data + header.header_length;
    size_t payload_size = header.remaining_length;

    switch (header.packet_type)
    {
        case mqtt_packet_type::CONNECT:
            return connect_packet::parse(payload, payload_size);
        // Other parse functions...
        default:
            throw std::runtime_error("Unsupported packet type");
    }
}

// Parse byte stream to a fixed header format.
fixed_header fixed_header::parse(const uint8_t* data, size_t size)
{   
    // Parse first byte of the packet.
    uint8_t byte1 = data[0];
    uint8_t packet_type_raw = (byte1 & 0xF0) >> 4;
    uint8_t flags = byte1 & 0x0F;


    uint32_t remaining_length = 0;
    int multiplier = 1;
    size_t index = 1;
    
    // Parse remaining length of the packet
    while(true)
    {
        if(index >= size) throw std::runtime_error("Malformed remaining length.");
        
        // Read integer value from data
        uint8_t encoded_byte = data[index++];
        remaining_length += (encoded_byte & 0x7F) * multiplier;

        if(multiplier > 128 * 128 * 128) throw std::runtime_error("Malformed remaining length.");

        // Last byte
        if ((encoded_byte & 0x80) == 0) break;

        multiplier *= 128;
    }

    size_t header_length = index;

    return fixed_header
    {
        static_cast<mqtt_packet_type>(packet_type_raw),
        flags,
        remaining_length,
        header_length
    };
}

std::unique_ptr<connect_packet> connect_packet::parse(const uint8_t* payload, size_t length)
{

}