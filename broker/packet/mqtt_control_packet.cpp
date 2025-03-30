#include <stdexcept>
#include <iostream>
#include "mqtt_control_packet.h"
#include "fixed_header.h"

// Parse byte stream to a packet format.
std::unique_ptr<mqtt_control_packet> mqtt_control_packet::mqtt_control_packet::parse(const uint8_t* data, size_t size)
{
    // Throw runtime error if size is too small.
    if (size < 2) throw std::runtime_error("Packet too small");

    // Parse fixed header.
    fixed_header header = fixed_header::parse(data, size);
    
    // Debug fixed hedaer.
    header.print_fixed_header();

    // Variable header pointer.
    const uint8_t* p_variable_header = data + header.header_length;

    variable_header v_header;
    switch (header.packet_type)
    {
        case mqtt_packet_type::CONNECT:
            v_header = variable_header::parse(p_variable_header, header.remaining_length);
            v_header.print_variable_header();
            return connect_packet::parse(p_variable_header, header.remaining_length);
        // Other parse functions...
        default:
            throw std::runtime_error("Unsupported packet type");
    }
}

// Parse byte stream to a fixed header format.
fixed_header fixed_header::parse(const uint8_t* data, size_t size)
{   
    // Packet type and flags
    uint8_t byte1 = data[0];
    uint8_t packet_type_raw = (byte1 & 0xF0) >> 4;
    uint8_t flags = byte1 & 0x0F;


    uint32_t remaining_length = 0;
    int multiplier = 1;
    size_t index = 1;
    
    // Remaining length
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
    
    return fixed_header
    {
        static_cast<mqtt_packet_type>(packet_type_raw),
        flags,
        remaining_length,
        header_length
    };
}

void fixed_header::print_fixed_header()
{
    std::cout<<"type: "             << (int) this->packet_type << '\n';
    std::cout<<"flags: "            << (int) this->flags << '\n';
    std::cout<<"remaining_length: " << this->remaining_length << '\n';
    std::cout<<"header_length: "    << this->header_length << '\n';
}

variable_header variable_header::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Protocol Name Length (2 bytes)
    if (size < 2) throw std::runtime_error("Malformed variable header length. (Too short)");
    uint16_t protocol_name_length = (data[index] << 8) | data[index + 1];
    index += 2;
    // v3.1.1 protocol name exception handle
    

    // 2. Protocol Name (n bytes)
    if (size < index + protocol_name_length) throw std::runtime_error("Malformed variable header. (Wrong protocol name)");
    std::string protocol_name = std::string(reinterpret_cast<const char*>(&data[index]), protocol_name_length);
    index += protocol_name_length;

    // 3. Protocol Level (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header. (Wrong protocol level)");
    uint8_t protocol_level = data[index++];

    // 4. Connect Flags (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header. (Wrong connect flags)");
    uint8_t flags = data[index++];
    bool username_flag   = (flags & 0b10000000) != 0;
    bool password_flag   = (flags & 0b01000000) != 0;
    bool will_retain     = (flags & 0b00100000) != 0;
    uint8_t will_qos     = (flags & 0b00011000) >> 3;
    bool will_flag       = (flags & 0b00000100) != 0;
    bool clean_session   = (flags & 0b00000010) != 0;

    // 5. Keep Alive (2 bytes)
    if (size < index + 2) throw std::runtime_error("Malformed variable header. (Wrong keep alive)");
    uint16_t keep_alive = (data[index] << 8) | data[index + 1];
    index += 2;

    return variable_header
    {
        protocol_name,
        protocol_level,
        username_flag,
        password_flag,
        will_retain,
        will_qos,
        will_flag,
        clean_session,
        keep_alive
    };
}

void variable_header::print_variable_header()
{
    std::cout<<"protocol_name: "  << this->protocol_name<<'\n';
    std::cout<<"protocol_level: " << (int)this->protocol_level<<'\n';
    std::cout<<"username_flag: "  << this->connect_flags.username_flag<<'\n';
    std::cout<<"password_flag: "  << this->connect_flags.password_flag<<'\n';
    std::cout<<"will_retain: "    << this->connect_flags.will_retain<<'\n';
    std::cout<<"will_qos: "       << (int)this->connect_flags.will_qos<<'\n';
    std::cout<<"will_flag: "      << this->connect_flags.will_flag<<'\n';
    std::cout<<"clean_session: "  << this->connect_flags.clean_session<<'\n';
    std::cout<<"keep_alive: "     << this->keep_alive<<'\n';
}

std::unique_ptr<connect_packet> connect_packet::parse(const uint8_t* payload, size_t length)
{
    // Parse protocol name.

    // Parse protocol version number.

    // Parse connect flags.

    // Parse keep alive timer.
}