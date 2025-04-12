#include <stdexcept>
#include <iostream>
#include "packet_handler.h"
#include "../session/session_manager.h"
#include "../subscription/subscription_manager.h"

std::vector<uint8_t> packet_handler::handle(const uint8_t* data, size_t size, socket_broker* socket)
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
            return handle_subscribe(static_cast<subscribe_packet&>(*packet), socket);
        case mqtt_packet_type::DISCONNECT:
            return handle_disconnect(socket);
        default:
            throw std::runtime_error("Unhandlable packet type: " + static_cast<int>(packet->type()));
            return std::vector<uint8_t>();
    }
}

std::vector<uint8_t> packet_handler::handle_connect(connect_packet& packet, socket_broker* socket)
{
    packet.debug();

    // Get manager instnaces
    session_manager& session_mgr = session_manager::get_instance();

    bool session_present;
    uint8_t return_code = 0;

    // Version check
    if(packet.v_header.protocol_name != "MQTT" || packet.v_header.protocol_level != 0x04)
        return_code = 0x01;

    // Handle clean session
    if(packet.v_header.connect_flags.clean_session)
    {
        session_present = false;

        session_mgr.register_session(packet.client_id, std::make_unique<mqtt_session>(packet, std::move(socket)));
    }
    else
    {
        // Register new session if client not duplicated.
        if((session_mgr.has_session(packet.client_id)))
        {
            // Connect would be rejected(0x02) if values of new connect packet are different form stated session.
            // Later...

            session_present = true;
            
            session_mgr.open_session(packet.client_id, socket);

            // Send untransmitted messages.
            // Later...
        }
        else
        {
            session_present = false;

            session_mgr.register_session(packet.client_id, std::make_unique<mqtt_session>(packet, socket));
        }
    }

    // Debug session
    session_mgr.debug();

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

std::vector<uint8_t> packet_handler::handle_subscribe(subscribe_packet& packet, socket_broker* socket)
{
    // // Debug subscribe packet
    // packet.debug();

    // // Get manager instances.
    // session_manager& session_mgr = session_manager::get_instance();
    // subscription_manager& sub_mgr = subscription_manager::get_instance();

    // // 1. Add or update subscription
    // for(size_t i = 0; i < packet.topic_filter.size(); i++)
    // {
    //     sub_mgr.add_subscription(packet.topic_filter[i], socket.get_clinet_id(), packet.qos_request[i]);
    // }
    
    // // 2. Decide return code.
    // std::vector<uint8_t> return_code;
    // for(auto tf = packet.topic_filter.begin(); tf != packet.topic_filter.end(); tf++)
    // {

    //     auto subs = sub_mgr.get_subscribers(*tf);

    //     auto it = std::find_if(subs.begin(), subs.end(), [&](const subscription s)
    //     {
    //         return s.client_id == socket.get_clinet_id();
    //     });

    //     // Set return code.
    //     if(it != subs.end())
    //     {
    //         // Set return code as a qos level of the subscription.
    //         return_code.push_back(it->qos);
    //     }
    //     else
    //     {
    //         // Subscribe failure.
    //         return_code.push_back(0x80);
    //     }
    // }
    
    // // 3. Reply suback packet.
    // std::unique_ptr<suback_packet> suback = suback_packet::create(packet.v_header.packet_identifier, return_code);
    // auto bytes = suback->serialize();

    // // Debug suback packet
    // fixed_header::parse(bytes.data(), bytes.size()).debug();
    // suback->debug();

    // return bytes;
}

std::vector<uint8_t> packet_handler::handle_disconnect(socket_broker* socket)
{
    // Get manager instance.
    session_manager& session_mgr = session_manager::get_instance();

    // 1. Disconnect client and close socket.
    std::string client_id = socket->get_client_id();
    if(session_mgr.is_clean_session(client_id))
    {
        session_mgr.remove_session(client_id);
    }
    else
    {
        session_mgr.close_session(client_id);
        // Discard will message.
        // Later...
    }

    session_mgr.debug();

    return std::vector<uint8_t>();
}