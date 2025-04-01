#pragma once

#include <string>

struct variable_header
{
    // Protocol name according MQTT version
    std::string protocol_name;
    // Protocol level according MQTT version
    uint8_t protocol_level;

    struct
    {
        bool username_flag;
        bool password_flag;
        bool will_retain;
        uint8_t will_qos;
        bool will_flag;
        bool clean_session;
    } connect_flags;
    
    uint16_t keep_alive;

    static variable_header parse(const uint8_t* data, size_t size);
    void print_variable_header();
};