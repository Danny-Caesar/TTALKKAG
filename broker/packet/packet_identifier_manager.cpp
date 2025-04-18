#include <iostream>
#include "packet_identifier_manager.h"

// Initailization
uint16_t packet_identifier_manager::_packet_identifier = 0;
std::unordered_map<uint16_t, uint8_t> packet_identifier_manager::_packet_identifier_map;

packet_identifier_manager& packet_identifier_manager::get_instance()
{
    static packet_identifier_manager instance;
    return instance;
}

uint16_t packet_identifier_manager::issue_packet_identifier(const uint8_t qos)
{
    ++_packet_identifier;
    _packet_identifier_map[_packet_identifier] = qos;
    return _packet_identifier;
}

uint8_t packet_identifier_manager::get_packet_qos(const uint16_t packet_identifier)
{
    return _packet_identifier_map[packet_identifier];
}

void packet_identifier_manager::acknowledge_packet_identifier(const uint16_t packet_identifier)
{
    _packet_identifier_map.erase(packet_identifier);
}

void packet_identifier_manager::debug()
{
    std::cout << "Pid ackonweledge list\n";
    for(auto pid : _packet_identifier_map)
    {
        std::cout << pid.first << "(QoS: " << (int)pid.second << ")\n";
    }
    std::cout<<'\n';
}