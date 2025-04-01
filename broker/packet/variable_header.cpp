#include <iostream>
#include "variable_header.h"

variable_header variable_header::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Protocol Name Length (2 bytes)
    if (size < 2) throw std::runtime_error("Malformed variable header length. (Too short)");
    uint16_t protocol_name_len = (data[index] << 8) | data[index + 1];
    index += 2;

    // 2. Protocol Name (n bytes)
    if (size < index + protocol_name_len) throw std::runtime_error("Malformed variable header: Wrong protocol name");
    std::string protocol_name = std::string(reinterpret_cast<const char*>(&data[index]), protocol_name_len);
    index += protocol_name_len;

    // 3. Protocol Level (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header: Wrong protocol level");
    uint8_t protocol_level = data[index++];

    // 4. Connect Flags (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header: Wrong connect flags");
    uint8_t flags = data[index++];
    bool username_flag   = (flags & 0b10000000) != 0;
    bool password_flag   = (flags & 0b01000000) != 0;
    bool will_retain     = (flags & 0b00100000) != 0;
    uint8_t will_qos     = (flags & 0b00011000) >> 3;
    bool will_flag       = (flags & 0b00000100) != 0;
    bool clean_session   = (flags & 0b00000010) != 0;

    // 5. Keep Alive (2 bytes)
    if (size < index + 2) throw std::runtime_error("Malformed variable header: Wrong keep alive");
    uint16_t keep_alive = (data[index] << 8) | data[index + 1];
    index += 2;

    variable_header v_header = 
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

    v_header.print_variable_header();

    return v_header;
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