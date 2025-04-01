#include "variable_header.h"
#include "connect_packet.h"
#include "connack_packet.h"
#include "socket_broker.h"

void connect_packet::handle(socket_broker& broker)
{
    bool session_present = false;   // Rector after session developed
    uint8_t return_code = 0x00;

    auto connack = connack_packet::create(session_present, return_code);
    auto buf = connack->serialize();
    broker.write(buf.data(), buf.size());
}

std::unique_ptr<connect_packet> connect_packet::parse(const uint8_t* payload, size_t length)
{
    size_t index = 0;

    // 1. Variable Header
    variable_header v_header = variable_header::parse(payload, length);
    index += 2 + v_header.protocol_name.size() + 1 + 1 + 2; // Protocol name len + name + level + flags + keep alive

    auto packet = std::make_unique<connect_packet>();
    packet->clean_session = v_header.connect_flags.clean_session;

    // 2. Client Identifier
    if (index + 2 > length) throw std::runtime_error("Malformed payload: Client ID length missing");
    uint16_t client_id_len = (payload[index] << 8) | payload[index + 1];
    index += 2;

    if (index + client_id_len > length) throw std::runtime_error("Malformed payload: Client ID data missing");
    packet->client_id = std::string(reinterpret_cast<const char*>(&payload[index]), client_id_len);
    index += client_id_len;

    // 3. Will Topic (if Will Flag)
    std::string will_topic;
    if (v_header.connect_flags.will_flag)
    {
        if (index + 2 > length) throw std::runtime_error("Malformed payload: Will topic length missing");
        uint16_t will_topic_len = (payload[index] << 8) | payload[index + 1];
        index += 2;

        if (index + will_topic_len > length) throw std::runtime_error("Malformed payload: Will topic data missing");
        will_topic = std::string(reinterpret_cast<const char*>(&payload[index]), will_topic_len);
        index += will_topic_len;
    }

    // 4. Will Message (if Will Flag)
    std::string will_message;
    if (v_header.connect_flags.will_flag)
    {
        if (index + 2 > length) throw std::runtime_error("Malformed payload: Will message length missing");
        uint16_t will_msg_len = (payload[index] << 8) | payload[index + 1];
        index += 2;

        if (index + will_msg_len > length) throw std::runtime_error("Malformed payload: Will message data missing");
        will_message = std::string(reinterpret_cast<const char*>(&payload[index]), will_msg_len);
        index += will_msg_len;
    }

    // 5. User Name (if flag set)
    std::string user_name;
    if (v_header.connect_flags.username_flag)
    {
        if (index + 2 > length) throw std::runtime_error("Malformed payload: Username length missing");
        uint16_t user_len = (payload[index] << 8) | payload[index + 1];
        index += 2;

        if (index + user_len > length) throw std::runtime_error("Malformed payload: Username data missing");
        user_name = std::string(reinterpret_cast<const char*>(&payload[index]), user_len);
        index += user_len;
    }

    // 6. Password (if flag set)
    std::string password;
    if (v_header.connect_flags.password_flag)
    {
        if (index + 2 > length) throw std::runtime_error("Malformed payload: Password length missing");
        uint16_t pw_len = (payload[index] << 8) | payload[index + 1];
        index += 2;

        if (index + pw_len > length) throw std::runtime_error("Malformed payload: Password data missing");
        password = std::string(reinterpret_cast<const char*>(&payload[index]), pw_len);
        index += pw_len;
    }

    return packet;
}