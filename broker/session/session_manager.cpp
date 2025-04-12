#include <algorithm>
#include <iostream>
#include "session_manager.h"

// Container initailize.
std::unordered_map<std::string, std::unique_ptr<mqtt_session>> session_manager::_session_map;

session_manager& session_manager::get_instance()
{
    static session_manager instance;
    return instance;
}

void session_manager::register_session(const std::string& client_id, std::unique_ptr<mqtt_session> session)
{
    auto it = _session_map.find(client_id);

    // Update clean session.
    if(it != _session_map.end())
        _session_map[client_id]->clean_session = session->clean_session;

    // Check whether value differs.
    // later...

    // Register client.
    _session_map[client_id] = std::move(session);
}

void session_manager::remove_session(const std::string& client_id)
{
    _session_map[client_id]->socket->close();
    _session_map.erase(client_id);
}

void session_manager::open_session(const std::string& client_id, socket_broker* socket)
{
    _session_map[client_id]->open_session(socket);
}

void session_manager::close_session(const std::string& client_id)
{
    _session_map[client_id]->close_session();
}

bool session_manager::has_session(const std::string& client_id)
{
    auto session = _session_map.find(client_id);

    if(session != _session_map.end())
        return true;
    else
        return false;
}

bool session_manager::is_clean_session(const std::string& client_id)
{
    return _session_map[client_id]->clean_session;
}

void session_manager::debug()
{
    std::cout << "----Session list----\n";
    for(auto it = _session_map.begin(); it != _session_map.end(); it++)
    {
        std::cout << it->first << '\n';
    }
    std::cout<<'\n';
}

//
// legacy
//

// mqtt_session* session_manager::find_session(const std::string& client_id)
// {
//     auto it = _session_map.find(client_id);
//     if(it == _session_map.end())
//         return NULL;

//     it->second->debug();

//     return it->second;
// }