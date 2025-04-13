#include <vector>
#include <unordered_map>
#include "subscription.h"

class subscription_manager
{
public:
    static subscription_manager& get_instance();
    void add_subscription(const std::string& topic, const std::string& client_id, uint8_t qos);
    void remove_subscription(const std::string& topic, const std::string& client_id);
    std::vector<subscription> get_subscribers(const std::string& topic);

    void debug();
    void debug(const std::string& topic);
    // void debug(const std::string& client_id);
    // subscription get_subscribtion(const std::string& topic, const std::string& client_id);

private:
    subscription_manager() = default;
    static std::unordered_map<std::string, std::vector<subscription>> _subscription_map;

    subscription_manager(const subscription_manager&) = delete;
    subscription_manager operator=(const subscription_manager&) = delete;
};