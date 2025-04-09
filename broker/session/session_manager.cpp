#include <algorithm>
#include <iostream>
#include "session_manager.h"

// Container initailize.
std::unordered_map<std::string, socket_broker*> session_manager::client_map;

session_manager& session_manager::get_instance()
{
    static session_manager instance;
    return instance;
}

void session_manager::register_client(const std::string& client_id, socket_broker* broker)
{
    // Duplicated client check.
    if(client_map.find(client_id) != client_map.end()) return;

    // Register client.
    client_map[client_id] = broker;
}

socket_broker* session_manager::get_client(const std::string& client_id)
{
    if(client_map.find(client_id) != client_map.end())
        return NULL;
    
    return client_map[client_id];
}

void session_manager::remove_client(const std::string& client_id, socket_broker* broker)
{
    client_map.erase(client_id);
}

void session_manager::debug()
{
    std::cout << "----Client list----\n";
    for(auto c : client_map)
    {
        std::cout << c.first << '\n';
    }
    std::cout << '\n';
}