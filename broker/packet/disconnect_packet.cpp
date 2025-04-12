#include "disconnect_packet.h"

std::unique_ptr<disconnect_packet> disconnect_packet::parse(const uint8_t* data, size_t size)
{
    return std::make_unique<disconnect_packet>();
}