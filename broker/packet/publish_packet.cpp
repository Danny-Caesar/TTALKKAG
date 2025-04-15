#include <stdexcept>
#include "publish_packet.h"

std::unique_ptr<publish_packet> publish_packet::parse(const uint8_t* data, size_t size, uint8_t flags)
{
    size_t index = 0;
    auto packet = std::make_unique<publish_packet>();

    // 1. Fixed header
    packet->dup = 0;
    packet->qos = (flags >> 1) & 0x03;
    packet->retain = flags & 0x01;
    
    // 2. Variable header
    packet->v_header = publish_packet::variable_header::parse(data, size, packet->qos);
    index += 2 + packet->v_header.topic_name.length(); // topic filter len, topic filter
    if(packet->qos > 0) index += 2; // packet id

    // 3. Message
    packet->message = std::string(reinterpret_cast<const char*>(&data[index]), size - index);

    return packet;
}

void publish_packet::debug()
{
    std::cout << "----Publish packet----\n";
    v_header.debug();
    std::cout << "---Payload---\n";
    std::cout << "dup: "     << (int)dup     << '\n';
    std::cout << "QoS: "     << (int)qos     << '\n';
    std::cout << "retain: "  << (int)retain  << '\n';
    std::cout << "message: " << message      << "\n\n";
}

publish_packet::variable_header publish_packet::variable_header::parse(const uint8_t* data, size_t size, uint8_t qos)
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
    uint16_t packet_identifier = 0;
    if (qos > 0)
    {
        if (size < index + 2) throw std::runtime_error("Malformed variable header: Wrong packet identifier.");
        packet_identifier = (data[index] << 8) | data[index + 1];
        index += 2;
    }

    publish_packet::variable_header v_header
    {
        topic_name,
        packet_identifier
    };

    return v_header;
}

void publish_packet::variable_header::debug()
{
    std::cout << "---Variable header---\n";
    std::cout << "topic name: "        << this->topic_name        << '\n';
    std::cout << "packet identifier: " << this->packet_identifier << '\n';
}