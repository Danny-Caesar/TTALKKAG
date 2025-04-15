#include <stdexcept>
#include <iostream>
#include "packet_handler.h"
#include "../session/session_manager.h"
#include "../subscription/subscription_manager.h"

std::vector<uint8_t> packet_handler::handle(const uint8_t* data, size_t size, std::shared_ptr<socket_broker> socket)
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

std::vector<uint8_t> packet_handler::handle_connect(connect_packet& packet, std::shared_ptr<socket_broker> socket)
{
    packet.debug();

    // Get manager instnaces
    session_manager& ses_mgr = session_manager::get_instance();

    bool session_present;
    uint8_t return_code = 0;

    // Version check
    if(packet.v_header.protocol_name != "MQTT" || packet.v_header.protocol_level != 0x04)
    {
        return_code = 0x01;
    }

    // Handle clean session
    if(packet.v_header.connect_flags.clean_session)
    {
        session_present = false;

        ses_mgr.register_session(packet.client_id, std::make_unique<mqtt_session>(packet, std::move(socket)));
    }
    else
    {
        // Register new session if client not duplicated.
        if((ses_mgr.has_session(packet.client_id)))
        {
            // Connect would be rejected(0x02) if values of new connect packet are different form stated session.
            // Later...

            std::string client_id = packet.client_id;
            session_present = true;
            
            // Open session.
            ses_mgr.open_session(client_id, std::move(socket));

            // Send messages retained in session
            mqtt_session session = ses_mgr.get_session(client_id);
            session.flush_message();
        }
        else
        {
            session_present = false;

            ses_mgr.register_session(packet.client_id, std::make_unique<mqtt_session>(packet, std::move(socket)));
        }
    }

    // Debug session
    ses_mgr.debug();

    // Create reply packet.
    auto connack = connack_packet::create(session_present, return_code);
    auto bytes = connack->serialize();
    
    return bytes;
}

std::vector<uint8_t> packet_handler::handle_publish(publish_packet& packet)
{
    // Debug packet
    packet.debug();

    // Get manager instances.
    subscription_manager& sub_mgr = subscription_manager::get_instance();
    session_manager& ses_mgr = session_manager::get_instance();

    // Do retain things related to subscription manager.
    sub_mgr.retain_message(packet.v_header.topic_name, packet);
    
    // Get subscriptions related to the topic.
    std::vector<subscription> subs = sub_mgr.get_subscription(packet.v_header.topic_name);

    // Do retain things related to each subscriptions.
    for(subscription sub : subs)
    {
        // Get session of subscriber.
        mqtt_session session = ses_mgr.get_session(sub.client_id);
        
        // Decide QoS
        uint8_t qos = std::min((uint8_t)(packet.qos | 0x03), sub.qos);
        packet.qos = qos;

        if(session.client_connect)
        {
            // Client online.
            session.socket->send_packet(packet);
        }
        else
        {
            // Client offline.
            // Do retain things.
            session.retain_message(packet);
        }
    }

    // Do QoS ack things.
    return std::vector<uint8_t>();
}

std::vector<uint8_t> packet_handler::handle_subscribe(subscribe_packet& packet, std::shared_ptr<socket_broker> socket)
{
    // Debug subscribe packet.
    packet.debug();

    // Get manager instances.
    subscription_manager& sub_mgr = subscription_manager::get_instance();

    // 1. Add or update subscriptions.
    std::vector<uint8_t> return_code;
    for(size_t i = 0; i < packet.topic_filter.size(); i++)
    {
        // Validate topic filter.
        // Set return code 0x80 if topic filter not validate.

        // Add subscription.
        sub_mgr.add_subscription(packet.topic_filter[i], socket->get_client_id(), packet.qos_request[i]);

        // Decide return code.
        return_code.push_back(packet.qos_request[i]);

        // Debug subscription insertion.
        sub_mgr.debug(packet.topic_filter[i]);
    }
    
    // 2. Transmit retained messages
    for(size_t i = 0; i < packet.topic_filter.size(); i++)
    {
        // Get retained messages related to topics.
        publish_packet message = sub_mgr.get_retained_message(packet.topic_filter[i]);
        // Decide QoS and set flags.
        uint8_t qos = std::min((uint8_t)(message.qos | 0x03), packet.qos_request[i]);
        message.qos = qos;
        // Transmit messages
        socket->send_packet(message);
    }

    // 3. Reply suback packet.
    std::unique_ptr<suback_packet> suback = suback_packet::create(packet.v_header.packet_identifier, return_code);
    auto bytes = suback->serialize();

    // Debug suback packet
    fixed_header::parse(bytes.data(), bytes.size()).debug();
    suback->debug();

    return bytes;
}

std::vector<uint8_t> packet_handler::handle_disconnect(std::shared_ptr<socket_broker> socket)
{
    // Get manager instance.
    session_manager& ses_mgr = session_manager::get_instance();

    // 1. Disconnect client and close socket.
    std::string client_id = socket->get_client_id();
    if(ses_mgr.is_clean_session(client_id))
    {
        ses_mgr.remove_session(client_id);
    }
    else
    {
        ses_mgr.close_session(client_id);
        // Discard will message.
        // Later...
    }

    ses_mgr.debug();

    return std::vector<uint8_t>();
}