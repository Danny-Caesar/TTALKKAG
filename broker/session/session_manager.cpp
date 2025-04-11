#include <algorithm>
#include <iostream>
#include "session_manager.h"

// Container initailize.
std::unordered_map<std::string, mqtt_session*> session_manager::_session_map;

session_manager& session_manager::get_instance()
{
    static session_manager instance;
    return instance;
}

void session_manager::register_session(const std::string& client_id, mqtt_session* session)
{
    // Duplicated client check.
    if(_session_map.find(client_id) != _session_map.end()) return;

    // Register client.
    _session_map[client_id] = session;
}

void session_manager::remove_session(const std::string& client_id)
{
    // _session_map[client_id]->socket->close();
    _session_map.erase(client_id);
}

mqtt_session* session_manager::get_session(const std::string& client_id)
{
    if(_session_map.find(client_id) != _session_map.end())
        return NULL;
    
    return _session_map[client_id];
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