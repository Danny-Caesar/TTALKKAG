#include "pingresp_packet.h"

std::unique_ptr<pingresp_packet> pingresp_packet::create()
{
    return std::make_unique<pingresp_packet>();
}