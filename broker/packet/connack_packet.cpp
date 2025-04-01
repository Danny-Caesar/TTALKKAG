#include "connack_packet.h"

std::unique_ptr<connack_packet> connack_packet::create(bool session_present = false, uint8_t return_code = 0x00)
{
    auto packet = std::make_unique<connack_packet>();
    packet->session_present = session_present;
    packet->return_code = return_code;
    return packet;
}