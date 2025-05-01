#include "unsuback_packet.h"

std::unique_ptr<unsuback_packet> unsuback_packet::create(uint16_t packet_identifier)
{
    auto packet = std::make_unique<unsuback_packet>();
    packet->v_header.packet_identifier = packet_identifier;

    return packet;
}

void unsuback_packet::debug()
{
    std::cout << "----Unsuback packet----\n";
    v_header.debug();
    std::cout << '\n';
}

void unsuback_packet::variable_header::debug()
{
    std::cout << "---Variable header---\n";
    std::cout << "packet identifier: " << packet_identifier << '\n';
}