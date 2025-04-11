#include <string>
#include <unordered_map>
#include "socket_broker.h"
#include "mqtt_session.h"

class session_manager
{
public:
    static session_manager& get_instance();
    void register_session(const std::string& client_id, mqtt_session* session);
    void remove_session(const std::string& client_id);
    void open_session(socket_broker* socket);
    void close_session();
    mqtt_session* get_session(const std::string& client_id);
    void debug();

private:
    session_manager() = default;
    session_manager(const session_manager&) = delete;
    session_manager operator=(const session_manager&) = delete;
    
    static std::unordered_map<std::string, mqtt_session*> _session_map;
};