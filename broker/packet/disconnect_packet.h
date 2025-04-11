#include "mqtt_control_packet.h"

class disconnect_packet : public mqtt_control_packet
{
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::DISCONNECT; }
    
    static std::unique_ptr<disconnect_packet> parse(const uint8_t* data, size_t size);
};