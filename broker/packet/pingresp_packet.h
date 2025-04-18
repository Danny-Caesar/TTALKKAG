#include <vector>
#include "mqtt_control_packet.h"

class pingresp_packet : public mqtt_control_packet
{
public:
    mqtt_packet_type type() const override { return mqtt_packet_type::PINGRESP; }
    static std::unique_ptr<pingresp_packet> create();

    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;
        // 1. Fixed header
        packet.push_back(0xD0); // (PINGRESP 13 << 4), flag
        packet.push_back(0x00); // Remaining length

        return packet;
    }
};