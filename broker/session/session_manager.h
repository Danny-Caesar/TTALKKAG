#include <string>
#include <unordered_map>
#include "socket_broker.h"

class session_manager
{
public:
    static session_manager& get_instance();
    void register_client(const std::string& client_id, socket_broker* broker);
    socket_broker* get_client(const std::string& client_id);
    void remove_client(const std::string& client_id, socket_broker* broker);
    void debug();

private:
    session_manager() = default;

    static std::unordered_map<std::string, socket_broker*> _session_map;

    session_manager(const session_manager&) = delete;
    session_manager operator=(const session_manager&) = delete;
};