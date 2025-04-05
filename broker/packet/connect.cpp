#include "connect.h"
#include "connack.h"
#include "socket_broker.h"

void connect::handle(socket_broker& broker)
{
    bool session_present = false;   // Rector after session developed
    uint8_t return_code = 0x00;

    auto connack = connack_packet::create(session_present, return_code);
    auto buf = connack->serialize();
    broker.write(buf.data(), buf.size());
}

std::unique_ptr<connect> connect::parse(const uint8_t* payload, size_t length)
{
    size_t index = 0;

    // 1. Variable header
    connect::variable_header v_header = variable_header::parse(payload, length);
    index += 2 + v_header.protocol_name.size() + 1 + 1 + 2; // Protocol name len + name + level + flags + keep alive

    auto packet = std::make_unique<connect>();
    packet->clean_session = v_header.connect_flags.clean_session;

    // 2. Client identifier
    if (index + 2 > length) throw std::runtime_error("Malformed payload: Client ID length missing");
    uint16_t client_id_len = (payload[index] << 8) | payload[index + 1];
    index += 2;

    if (index + client_id_len > length) throw std::runtime_error("Malformed payload: Client ID data missing");
    packet->client_id = std::string(reinterpret_cast<const char*>(&payload[index]), client_id_len);
    index += client_id_len;

    // 3. Will topic (if will flag)
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

    // 4. Will message (if will flag)
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

    // 5. User name (if flag set)
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

connect::variable_header connect::variable_header::parse(const uint8_t* data, size_t size)
{
    size_t index = 0;

    // 1. Protocol name length (2 bytes)
    if (size < 2) throw std::runtime_error("Malformed variable header length. (Too short)");
    uint16_t protocol_name_len = (data[index] << 8) | data[index + 1];
    index += 2;

    // 2. Protocol name (n bytes)
    if (size < index + protocol_name_len) throw std::runtime_error("Malformed variable header: Wrong protocol name");
    std::string protocol_name = std::string(reinterpret_cast<const char*>(&data[index]), protocol_name_len);
    index += protocol_name_len;

    // 3. Protocol level (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header: Wrong protocol level");
    uint8_t protocol_level = data[index++];

    // 4. Connect flags (1 byte)
    if (size < index + 1) throw std::runtime_error("Malformed variable header: Wrong connect flags");
    uint8_t flags = data[index++];
    bool username_flag   = (flags & 0b10000000) != 0;
    bool password_flag   = (flags & 0b01000000) != 0;
    bool will_retain     = (flags & 0b00100000) != 0;
    uint8_t will_qos     = (flags & 0b00011000) >> 3;
    bool will_flag       = (flags & 0b00000100) != 0;
    bool clean_session   = (flags & 0b00000010) != 0;

    // 5. Keep alive (2 bytes)
    if (size < index + 2) throw std::runtime_error("Malformed variable header: Wrong keep alive");
    uint16_t keep_alive = (data[index] << 8) | data[index + 1];
    index += 2;

    variable_header v_header = 
    {
        protocol_name,
        protocol_level,
        username_flag,
        password_flag,
        will_retain,
        will_qos,
        will_flag,
        clean_session,
        keep_alive
    };

    v_header.debug();

    return v_header;
}

void connect::variable_header::debug()
{
    std::cout << "----Variable Header----\n";
    std::cout << "protocol_name: "  << this->protocol_name               << '\n';
    std::cout << "protocol_level: " << (int)this->protocol_level         << '\n';
    std::cout << "username_flag: "  << this->connect_flags.username_flag << '\n';
    std::cout << "password_flag: "  << this->connect_flags.password_flag << '\n';
    std::cout << "will_retain: "    << this->connect_flags.will_retain   << '\n';
    std::cout << "will_qos: "       << (int)this->connect_flags.will_qos << '\n';
    std::cout << "will_flag: "      << this->connect_flags.will_flag     << '\n';
    std::cout << "clean_session: "  << this->connect_flags.clean_session << '\n';
    std::cout << "keep_alive: "     << this->keep_alive                  << "\n\n";
}