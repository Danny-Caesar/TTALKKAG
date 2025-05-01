#pragma once

#include <string>
#include <unordered_map>
#include "socket_broker.h"
#include "mqtt_session.h"

class session_manager
{
public:
    static session_manager& get_instance();
    void register_session(const std::string& client_id, std::unique_ptr<mqtt_session> session);
    void remove_session(const std::string& client_id);
    mqtt_session& get_session(const std::string& client_id);
    void open_session(const std::string& client_id, std::shared_ptr<socket_broker> socket);
    void close_session(const std::string& client_id);
    bool has_session(const std::string& client_id);
    bool is_clean_session(const std::string& client_id);
    void debug();

    // mqtt_session* find_session(const std::string& client_id);

private:
    session_manager() = default;
    session_manager(const session_manager&) = delete;
    session_manager operator=(const session_manager&) = delete;
    
    static std::unordered_map<std::string, std::unique_ptr<mqtt_session>> _session_map;
};