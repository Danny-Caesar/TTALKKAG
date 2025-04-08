#include "subscribe_packet.h"
#include "suback_packet.h"
#include "socket_broker.h"

std::unique_ptr<subscribe_packet> subscribe_packet::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Variable header
    if (size < 2) throw std::runtime_error("Malformed payload: Wrong variable header.");
    subscribe_packet::variable_header v_header = subscribe_packet::variable_header::parse(data, size);
    index += 2;

    auto packet = std::make_unique<subscribe_packet>();
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

std::unique_ptr<subscribe_packet> subscribe_packet::create(
    uint16_t packet_identifier,
    std::vector<std::string> topic_filter,
    std::vector<uint8_t> qos_request
)
{
    auto packet = std::make_unique<subscribe_packet>();
    packet->v_header.packet_identifier = packet_identifier;
    packet->topic_filter = topic_filter;
    packet->qos_request = qos_request;

    return packet;
}

void subscribe_packet::debug()
{
    std::cout << "----Subscribe packet----\n";
    v_header.debug();
    std::cout << "---Payload---\n";
    for(size_t i = 0; i < this->topic_filter.size(); i++)
    {
        std::cout << "topic filter: "  << this->topic_filter[i] << '\n';
        std::cout << "requested QoS: " << (int)this->qos_request[i]  << '\n';
    }
    std::cout << '\n';
}

subscribe_packet::variable_header subscribe_packet::variable_header::parse(const uint8_t* data, size_t size)
{
    // 1. Packet identifier
    if (size < 2) throw std::runtime_error("Malformed varialbe header: Wrong packet identifier.");
    uint16_t packet_id = (data[0] << 8) | data[1];

    return subscribe_packet::variable_header{ packet_id };
}

void subscribe_packet::variable_header::debug()
{
    std::cout << "---Variable header---\n";
    std::cout << "packet id: " << this->packet_identifier << "\n\n";
}