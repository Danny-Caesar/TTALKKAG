#include "subscribe.h"

void subscribe::handle(socket_broker& broker)
{
    this->debug();
}

std::unique_ptr<subscribe> subscribe::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Variable header
    if (size < 2) throw std::runtime_error("Malformed payload: Wrong variable header.");
    subscribe::variable_header v_header = subscribe::variable_header::parse(data, size);
    index += 2;

    auto packet = std::make_unique<subscribe>();
    packet->v_header.packet_identifier = v_header.packet_identifier;

    while(index < size)
    {
        // 2. Length
        if (size < index + 2) throw std::runtime_error("Malformed payload: Wrong topic filter length.");
        uint16_t length = (data[index] << 8) | data[index + 1];
        index += 2;
        
        // 3. Topic filter
        if (size < index + length) throw std::runtime_error("Malformed payload: Wrong topic filter.");
        packet->topic_filter.push_back(std::string(reinterpret_cast<const char*>(&data[index]), length));
        index += length;

        // 4. Requested QoS
        if (size < index + 1) throw std::runtime_error("Malformed payload: Wrong QoS request.");
        packet->qos_request.push_back(data[index]);
        index += 1;
    }

    return packet;
}

std::unique_ptr<subscribe> subscribe::create(
    uint16_t packet_identifier,
    std::vector<std::string> topic_filter,
    std::vector<uint8_t> qos_request
)
{
    auto packet = std::make_unique<subscribe>();
    packet->v_header.packet_identifier = packet_identifier;
    packet->topic_filter = topic_filter;
    packet->qos_request = qos_request;

    return packet;
}

void subscribe::debug()
{
    std::cout << "----Payload----\n";
    for(size_t i = 0; i < this->topic_filter.size(); i++)
    {
        std::cout << "topic filter: "  << this->topic_filter[i] << '\n';
        std::cout << "requested QoS: " << this->qos_request[i]  << '\n';
    }
    std::cout << '\n';
}

subscribe::variable_header subscribe::variable_header::parse(const uint8_t* data, size_t size)
{
    // 1. Packet identifier
    if (size < 2) throw std::runtime_error("Malformed varialbe header: Wrong packet identifier.");
    uint16_t packet_id = (data[0] << 8) | data[1];

    return subscribe::variable_header{ packet_id };
}