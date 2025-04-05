#include <stdexcept>
#include "publish.h"

std::unique_ptr<publish> publish::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;
    auto packet = std::make_unique<publish>();
    
    // 1. Variable header
    packet->v_header = publish::variable_header::parse(data, size);
    index += packet->v_header.topic_name.length() + 2;

    // 2. Message
    packet->message = std::string(reinterpret_cast<const char*>(&data[index]), size - index);

    return packet;
}

void publish::handle(socket_broker& broker)
{
    std::cout << "message: " << this->message << "\n\n";
}

publish::variable_header publish::variable_header::parse(const uint8_t* data, size_t size)
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

    publish::variable_header v_header
    {
        topic_name,
        packet_identifier
    };

    v_header.debug();

    return v_header;
}

void publish::variable_header::debug()
{
    std::cout << "----Variable Header----\n";
    std::cout << "topic name: "        << this->topic_name        << '\n';
    std::cout << "packet identifier: " << this->packet_identifier << "\n\n";
}