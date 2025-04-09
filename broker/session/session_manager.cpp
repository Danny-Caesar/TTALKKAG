#include <algorithm>
#include <iostream>
#include "session_manager.h"

// Container initailize.
std::unordered_map<std::string, socket_broker*> session_manager::_session_map;

session_manager& session_manager::get_instance()
{
    static session_manager instance;
    return instance;
}

void session_manager::register_client(const std::string& client_id, socket_broker* broker)
{
    // Duplicated client check.
    if(_session_map.find(client_id) != _session_map.end()) return;

    // Register client.
    _session_map[client_id] = broker;
}

socket_broker* session_manager::get_client(const std::string& client_id)
{
    if(_session_map.find(client_id) != _session_map.end())
        return NULL;
    
    return _session_map[client_id];
}

void session_manager::remove_client(const std::string& client_id, socket_broker* broker)
{
    _session_map.erase(client_id);
}

void session_manager::debug()
{
    std::cout << "----Client list----\n";
    for(auto c : _session_map)
    {
        std::cout << c.first << '\n';
    }
    std::cout << '\n';
}