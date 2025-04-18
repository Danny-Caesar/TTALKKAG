#include "puback_packet.h"

std::unique_ptr<puback_packet> puback_packet::parse(const uint8_t* data, size_t size)
{
    auto packet = std::make_unique<puback_packet>();
    
    // 1. Variable header
    packet->v_header = variable_header::parse(data, size);

    return packet;
}

std::unique_ptr<puback_packet> puback_packet::create(uint16_t packet_identifier)
{
    auto packet = std::make_unique<puback_packet>();

    packet->v_header.packet_identifier = packet_identifier;

    return packet;
}

puback_packet::variable_header puback_packet::variable_header::parse(const uint8_t* data, size_t size)
{
    if(size < 2) std::__throw_runtime_error("Malformed puback: too short.");

    uint16_t packet_id = (data[0] << 8) | data[1];

    return puback_packet::variable_header{ packet_id };
}

void puback_packet::debug()
{
    std::cout << "----Puback packet----\n";
    v_header.debug();
    std::cout << '\n';
}

void puback_packet::variable_header::debug()
{
    std::cout << "---Variable header---\n";
    std::cout << "packet identifier: " << packet_identifier << '\n';
}