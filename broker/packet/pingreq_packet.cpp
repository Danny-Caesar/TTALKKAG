#include "pingreq_packet.h"

std::unique_ptr<pingreq_packet> pingreq_packet::parse(const uint8_t* data, size_t size)
{
    return std::make_unique<pingreq_packet>();
}