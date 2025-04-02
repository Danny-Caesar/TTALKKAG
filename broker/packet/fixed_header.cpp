#include "fixed_header.h"
#include <iostream>

// Parse byte stream to a fixed header format.
fixed_header fixed_header::parse(const uint8_t* data, size_t size)
{   
    // 1. Packet type and flags
    uint8_t byte1 = data[0];
    uint8_t packet_type_raw = (byte1 & 0xF0) >> 4;
    uint8_t flags = byte1 & 0x0F;


    // 2. Remaining length
    uint32_t remaining_length = 0;
    int multiplier = 1;
    size_t index = 1;
    
    while(true)
    {
        if(index >= size) throw std::runtime_error("Malformed remaining length. (Too short)");
        
        // Read integer value from data
        uint8_t encoded_byte = data[index++];
        remaining_length += (encoded_byte & 0x7F) * multiplier;

        if(multiplier > 128 * 128 * 128) throw std::runtime_error("Malformed remaining length. (Too long)");

        // Last byte
        if ((encoded_byte & 0x80) == 0) break;

        multiplier *= 128;
    }

    size_t header_length = index;
    
    fixed_header f_header
    {
        static_cast<mqtt_packet_type>(packet_type_raw),
        flags,
        remaining_length,
        header_length
    };

    f_header.debug();

    return f_header;
}

void fixed_header::debug()
{
    std::cout << "----Fixed Header----\n";
    std::cout << "type: "             << (int) this->packet_type << '\n';
    std::cout << "flags: "            << (int) this->flags       << '\n';
    std::cout << "remaining_length: " << this->remaining_length  << '\n';
    std::cout << "header_length: "    << this->header_length     << "\n\n";
}