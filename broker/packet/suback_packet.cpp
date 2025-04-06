#include "suback_packet.h"

std::unique_ptr<suback_packet> suback_packet::create(uint16_t packet_identifier, std::vector<uint8_t> return_code)
{
    auto packet = std::make_unique<suback_packet>();

    // 1. Variable header
    // Variable header
    packet->v_header.packet_identifier = packet_identifier;

    // 2. Payload
    // return code
    packet->return_code = return_code;

    return packet;
}

void suback_packet::debug()
{
    std::cout << "----Suback packet----\n";
    v_header.debug();
    std::cout << "---Payload---\n";
    std::cout << "return code: [ ";
    for(auto rc : return_code)
        std::cout << (int)rc << ' ';
    std::cout << "]\n\n";
}

void suback_packet::variable_header::debug()
{
    std::cout << "---Variable Header---\n";
    std::cout << "packet identifier: " << (int)packet_identifier << "\n\n";
}