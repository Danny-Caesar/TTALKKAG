#include "packet_handler.h"
#include <stdexcept>
#include <iostream>

std::vector<uint8_t> packet_handler::handle(const uint8_t* data, size_t size)
{
    std::unique_ptr<mqtt_control_packet> packet = mqtt_control_packet::parse(data, size);
    if(packet==NULL) return std::vector<uint8_t>();

    switch (packet->type())
    {
        case mqtt_packet_type::CONNECT:
            return handle_connect(static_cast<connect_packet&>(*packet));

        case mqtt_packet_type::PUBLISH:
            return handle_publish(static_cast<publish_packet&>(*packet));

        case mqtt_packet_type::SUBSCRIBE:
            return handle_subscribe(static_cast<subscribe_packet&>(*packet));
        default:
            std::cerr << "[packet_handler] Unsupported packet type: " << static_cast<int>(packet->type()) << std::endl;
            return std::vector<uint8_t>();
    }
}

std::vector<uint8_t> packet_handler::handle_connect(connect_packet& packet)
{
    bool session_present = false;
    if(packet.v_header.connect_flags.clean_session)
        session_present = false;
    uint8_t return_code = 0x00;

    auto connack = connack_packet::create(session_present, return_code);
    auto bytes = connack->serialize();
    
    return bytes;
}

std::vector<uint8_t> packet_handler::handle_publish(publish_packet& packet)
{
    std::cout << "message: " << packet.message << "\n\n";

    return std::vector<uint8_t>();
}

std::vector<uint8_t> packet_handler::handle_subscribe(subscribe_packet& packet)
{
    // Debug subscribe packet
    packet.debug();

    std::unique_ptr<suback_packet> suback;
    
    // 1. Return code
    std::vector<uint8_t> return_code;
    for(size_t i = 0; i < packet.topic_filter.size(); i++)
    {
        bool failure = false;
        if(failure)
            // Subscribe Failure
            return_code.push_back(0x80);
        else
            return_code.push_back(packet.qos_request[i]);
    }

    
    // 2. Transmit
    suback = suback_packet::create(packet.v_header.packet_identifier, return_code);
    auto bytes = suback->serialize();

    // Debug suback packet
    fixed_header::parse(bytes.data(), bytes.size()).debug();
    suback->debug();

    return bytes;
}
