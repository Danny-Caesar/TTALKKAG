#pragma once

#include <vector>
#include <unordered_map>
#include "subscription.h"
#include "publish_packet.h"

class subscription_manager
{
public:
    static subscription_manager& get_instance();

    void add_subscription(const std::string& topic, const std::string& client_id, uint8_t qos);
    void remove_subscription(const std::string& topic, const std::string& client_id);
    
    std::vector<subscription> get_subscription(const std::string& topic);
    subscription* get_subscription(const std::string& topic, const std::string& client_id);
    std::vector<std::string> get_topics(const std::string& client_id);

    void retain_message(const std::string& topic, publish_packet& packet);
    void remove_message(const std::string& topic);
    publish_packet get_retained_message(const std::string& topic);

    void debug();
    void debug(const std::string& topic);
    // void debug(const std::string& client_id);
    // subscription get_subscribtion(const std::string& topic, const std::string& client_id);

private:
    subscription_manager() = default;
    subscription_manager(const subscription_manager&) = delete;
    subscription_manager operator=(const subscription_manager&) = delete;

    // topic name -> subscriptions
    static std::unordered_map<std::string, std::vector<subscription>> _subscription_map;
    // topic name -> publish packet
    static std::unordered_map<std::string, publish_packet> _message_map;
};