#include <stdexcept>
#include <iostream>
#include "packet_handler.h"
#include "../session/session_manager.h"
#include "../subscription/subscription_manager.h"

std::vector<uint8_t> packet_handler::handle(const uint8_t* data, size_t size, socket_broker& socket)
{
    std::unique_ptr<mqtt_control_packet> packet = mqtt_control_packet::parse(data, size);
    if(packet==NULL) return std::vector<uint8_t>();

    switch (packet->type())
    {
        case mqtt_packet_type::CONNECT:
            return handle_connect(static_cast<connect_packet&>(*packet), socket);

        case mqtt_packet_type::PUBLISH:
            return handle_publish(static_cast<publish_packet&>(*packet));

        case mqtt_packet_type::SUBSCRIBE:
            return handle_subscribe(static_cast<subscribe_packet&>(*packet));
        default:
        throw std::runtime_error("Unhandlable packet type: " + static_cast<int>(packet->type()));
            return std::vector<uint8_t>();
    }
}

std::vector<uint8_t> packet_handler::handle_connect(connect_packet& packet, socket_broker& socket)
{
    packet.debug();

    session_manager& sm = session_manager::get_instance();
    bool session_present;
    uint8_t return_code = 0;

    // Version check
    if(packet.v_header.protocol_name != "MQTT" || packet.v_header.protocol_level != 0x04)
        return_code = 1;

    // Handle clean session
    if(packet.v_header.connect_flags.clean_session)
    {
        session_present = false;
        sm.remove_client(packet.client_id, &socket);
    }
    else
    {
        // Register new session if client not duplicated.
        if(sm.get_client(packet.client_id) != NULL)
        {
            session_present = true;
            return_code = 2;
        }
        else
        {
            session_present = false;
            sm.register_client(packet.client_id, &socket);
        }
    }
    
    // Debug session
    sm.debug();

    // Create reply packet.
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

    // for(size_t i = 0; i < packet.topic_filter.size(); i++)
    // {
    //     subscription_manager::add_subscription(packet.topic_filter[i], session.client_id, packet.qos_request[i])

    // }

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
