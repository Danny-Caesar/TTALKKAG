#include "mqtt_control_packet.h"

class pingreq_packet : public mqtt_control_packet
{
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::PINGREQ; }
    static std::unique_ptr<pingreq_packet> parse(const uint8_t* data, size_t size);
};