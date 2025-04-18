#include "packet_identifier_manager.h"

// Initailization
uint16_t packet_identifier_manager::_packet_identifier = 0;
std::unordered_map<uint16_t, uint8_t> packet_identifier_manager::_packet_identifier_map;

packet_identifier_manager& packet_identifier_manager::get_instance()
{
    static packet_identifier_manager instance;
    return instance;
}

const uint16_t packet_identifier_manager::issue_packet_identifier(const uint8_t qos)
{
    _packet_identifier_map[_packet_identifier++] = qos;
}

const uint8_t packet_identifier_manager::get_packet_qos(const uint16_t packet_identifier)
{
    return _packet_identifier_map[packet_identifier];
}

void packet_identifier_manager::acknowledge_packet_identifier(const uint16_t packet_identifier)
{
    _packet_identifier_map.erase(packet_identifier);
}

