#include "unsubscribe_packet.h"

std::unique_ptr<unsubscribe_packet> unsubscribe_packet::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Variable header
    if (size < 2) throw std::runtime_error("Malformed payload: Wrong variable header.");
    unsubscribe_packet::variable_header v_header = unsubscribe_packet::variable_header::parse(data, size);
    index += 2;

    auto packet = std::make_unique<unsubscribe_packet>();
    packet->v_header.packet_identifier = v_header.packet_identifier;

    // 2. Payload
    while(index < size)
    {
        // Length
        if (size < index + 2) throw std::runtime_error("Malformed payload: Wrong topic filter length.");
        uint16_t length = (data[index] << 8) | data[index + 1];
        index += 2;

        // Topic filter
        if (size < index + length) throw std::runtime_error("Malformed payload: Wrong topic filter.");
        packet->topic_filter.push_back(std::string(reinterpret_cast<const char*>(&data[index]), length));
        index += length;
    }

    return packet;
}

unsubscribe_packet::variable_header unsubscribe_packet::variable_header::parse(const uint8_t* data, size_t size)
{
    // 1. Packet identifier
    if (size < 2) throw std::runtime_error("Malformed varialbe header: Wrong packet identifier.");
    uint16_t packet_id = (data[0] << 8) | data[1];

    return unsubscribe_packet::variable_header{ packet_id };
}

void unsubscribe_packet::debug()
{
    std::cout << "----Unsubscribe packet----\n";
    v_header.debug();
    std::cout << "---Payload---\n";
    std::cout << "--topic filter--\n";
    for(auto tf : topic_filter)
    {
        std::cout << tf << '\n';
    }
    std::cout << '\n';
}

void unsubscribe_packet::variable_header::debug()
{
    std::cout << "---Variable header---\n";
    std::cout << "packet identifier: " << packet_identifier << '\n';
}