#pragma once

#include "mqtt_control_packet.h"
#include <string>

class connect_packet : public mqtt_control_packet
{
    public:
        std::string client_id;
        std::string will_topic;
        std::string will_message;
        std::string user_name;
        std::string password;

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
            void debug();
        } v_header;
    public:
        mqtt_packet_type type() const override { return mqtt_packet_type::CONNECT; }
        void debug();

        static std::unique_ptr<connect_packet> parse(const uint8_t* data, size_t size);
};
