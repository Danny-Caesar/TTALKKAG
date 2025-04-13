#include "subscription_manager.h"
#include <algorithm>

// Initalization
std::unordered_map<std::string, std::vector<subscription>> subscription_manager::mqtt_subscriptions;

// Return singleton instance.
subscription_manager& subscription_manager::get_instance()
{
    static subscription_manager instance;
    return instance;
}

// Add or update subscription.
// Return return code of suback packet.
void subscription_manager::add_subscription(const std::string& topic, const std::string& client_id, uint8_t qos)
{
    // Get reference of the list of subscriptions for the topic
    auto& subs = mqtt_subscriptions[topic];

    auto it = std::find_if(subs.begin(), subs.end(), [&](const subscription& s)
    {
        return s.client_id == client_id;
    });
    
    // Duplication check
    if (it != subs.end())
    {
        // If already exists, just update QoS.
        it->qos = qos;
    }
    else
    {
        // Add new subscription.
        subs.push_back(subscription{ client_id, qos });
    }
}

// Remove subscription.
void subscription_manager::remove_subscription(const std::string& topic, const std::string& client_id)
{
    // Check whether the topic exists in the subscription map.
    auto it = mqtt_subscriptions.find(topic);
    if (it == mqtt_subscriptions.end()) return; // No such topic.

    // Get reference to the list of subscriptions for this topic.
    auto& subs = it->second;

    // Remove the subscription with the matching client_id.
    subs.erase(
        std::remove_if(subs.begin(), subs.end(), [&](const subscription& s) {
            return s.client_id == client_id;
        }),
        subs.end()
    );

    // If the subscription list is now empty, remove the topic entry.
    if (subs.empty())
    {
        mqtt_subscriptions.erase(it);
    }
}

// Return subscribers of specific topic.
std::vector<subscription> subscription_manager::get_subscribers(const std::string& topic)
{
    // Check if the topic exists in the subscription map
    auto it = mqtt_subscriptions.find(topic);
    if (it == mqtt_subscriptions.end())
        return std::vector<subscription>();

    return it->second;
}

// Return specific subscription.
subscription subscription_manager::get_subscribtion(const std::string& topic, const std::string& client_id)
{
    std::vector<subscription> sub = mqtt_subscriptions[topic];
    
    auto it = std::find_if(sub.begin(), sub.end(), [&](const subscription s)
    {
        return s.client_id == client_id;
    });

    return *it;
}