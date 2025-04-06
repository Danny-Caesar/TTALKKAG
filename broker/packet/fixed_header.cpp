#include "fixed_header.h"
#include <iostream>

// Parse byte stream to a fixed header format.
fixed_header fixed_header::parse(const uint8_t* data, size_t size)
{   
    // 1. Packet type and flags
    uint8_t byte1 = data[0];
    uint8_t packet_type_raw = (byte1 & 0xF0) >> 4;
    uint8_t flags = byte1 & 0x0F;

    size_t index = 1;

    // 2. Remaining length
    std::pair<uint32_t, size_t> remaining_length = fixed_header::decode_remaining_length(&data[index], size - index);

    size_t header_length = index + remaining_length.second;

    return fixed_header
    {
        static_cast<mqtt_packet_type>(packet_type_raw),
        flags,
        remaining_length.first,
        header_length
    };
}

std::pair<uint32_t, size_t> fixed_header::decode_remaining_length(const uint8_t* data, size_t size)
{
    uint32_t remaining_length = 0;
    int multiplier = 1;
    size_t index = 0;

    while(true)
    {   
        if(size < index + 1) throw std::runtime_error("Malformed remaining length: Too short.");

        // Read integer value from data
        uint8_t encoded_byte = data[index++];
        remaining_length += (encoded_byte & 0x7F) * multiplier;

        if(multiplier > 128 * 128 * 128) throw std::runtime_error("Malformed remaining length: Too long.");

        // Last byte
        if ((encoded_byte & 0x80) == 0) break;

        multiplier *= 128;
    }

    return std::pair<uint32_t, size_t>(remaining_length, index);
}

std::vector<uint8_t> fixed_header::encode_remaining_length(uint32_t remaining_length)
{
    std::vector<uint8_t> buf;

    do {
        uint8_t encoded_byte = remaining_length % 128;
        remaining_length /= 128;
        if (remaining_length > 0)
            encoded_byte |= 0x80;
        buf.push_back(encoded_byte);
    } while (remaining_length > 0);

    return buf;
}

void fixed_header::debug()
{
    std::cout << "---Fixed Header---\n";
    std::cout << "type: "             << (int)this->packet_type  << '\n';
    std::cout << "flags: [ ";
    for(int i = 3; i >= 0; i--)
        std::cout << (int)((this->flags >> i) & 0x01) << ' ';
    std::cout << "] \n";
    std::cout << "remaining_length: " << this->remaining_length  << '\n';
    std::cout << "header_length: "    << this->header_length     << "\n\n";
}