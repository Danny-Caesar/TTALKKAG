#include "connack_packet.h"

std::unique_ptr<connack_packet> connack_packet::create(bool session_present, uint8_t return_code)
{
    auto packet = std::make_unique<connack_packet>();
    packet->v_header.session_present = session_present;
    packet->v_header.return_code = return_code;
    return packet;
}