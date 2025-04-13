#include <stdexcept>
#include "publish_packet.h"

std::unique_ptr<publish_packet> publish_packet::parse(const uint8_t* data, size_t size, uint8_t flags)
{
    size_t index = 0;
    auto packet = std::make_unique<publish_packet>();

    // 1. Fixed header
    packet->_flags = flags;
    
    // 2. Variable header
    packet->v_header = publish_packet::variable_header::parse(data, size);
    index += packet->v_header.topic_name.length() + 2;

    // 3. Message
    packet->message = std::string(reinterpret_cast<const char*>(&data[index]), size - index);

    return packet;
}

void publish_packet::set_flags(uint8_t flags)
{
    _flags = flags;
}

uint8_t publish_packet::get_flags()
{
    return _flags;
}

void publish_packet::debug()
{
    std::cout << "----Publish packet----\n";
    v_header.debug();
    std::cout << "flags: "   << _flags  << '\n';
    std::cout << "message: " << message << "\n\n";
}

publish_packet::variable_header publish_packet::variable_header::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Topic name length (2 bytes)
    if (size < 2) throw std::runtime_error("Malformed variable header length: Wrong topic name length.");
    uint16_t topic_name_len = (data[index] << 8) | data[index + 1];
    index += 2;

    // 2. Topic name (n bytes)
    if (size < index + topic_name_len) throw std::runtime_error("Malformed variable header: Wrong topic name.");
    std::string topic_name = std::string(reinterpret_cast<const char*>(&data[index]), topic_name_len);
    index += topic_name_len;

    // 3. Packet identifier (2 bytes)
    if (size < index + 2) throw std::runtime_error("Malformed variable header: Wrong packet identifier.");
    uint16_t packet_identifier = (data[index] << 8) | data[index + 1];
    index += 2;

    publish_packet::variable_header v_header
    {
        topic_name,
        packet_identifier
    };

    v_header.debug();

    return v_header;
}

void publish_packet::variable_header::debug()
{
    std::cout << "----Variable header----\n";
    std::cout << "topic name: "        << this->topic_name        << '\n';
    std::cout << "packet identifier: " << this->packet_identifier << '\n';
}