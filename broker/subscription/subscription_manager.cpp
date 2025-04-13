#include <iostream>
#include <algorithm>
#include "subscription_manager.h"

// Initalization
std::unordered_map<std::string, std::vector<subscription>> subscription_manager::_subscription_map;

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
    auto& subs = _subscription_map[topic];

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
    auto it = _subscription_map.find(topic);
    if (it == _subscription_map.end()) return; // No such topic.

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
        _subscription_map.erase(it);
    }
}

// Return subscribers of specific topic.
std::vector<subscription> subscription_manager::get_subscribers(const std::string& topic)
{
    // Check if the topic exists in the subscription map
    auto it = _subscription_map.find(topic);
    if (it == _subscription_map.end())
        return std::vector<subscription>();

    return it->second;
}

void subscription_manager::debug()
{
    std::cout << "----Subscription list----\n";
    for(auto it_topic = _subscription_map.begin(); it_topic != _subscription_map.end(); it_topic++)
    {
        std::cout << "---" << it_topic->first << "---\n";
        for(auto it_client = it_topic->second.begin(); it_client != it_topic->second.end(); it_client++)
        {
            std::cout << it_client->client_id << '(' << (int)it_client->qos << ")\n";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void subscription_manager::debug(const std::string& topic)
{
    std::cout << "----Subscription list----\n";
    auto it_topic = _subscription_map.find(topic);

    std::cout << "---" << it_topic->first << "---\n";

    for(auto it_client = it_topic->second.begin(); it_client != it_topic->second.end(); it_client++)
    {
        std::cout << it_client->client_id << '(' << (int)it_client->qos << ")\n";
    }
    std::cout << '\n';
}

// void subscription_manager::debug(const std::string& client_id)
// {
//     std::cout << "----Subscription list----\n";
//     std::cout << "---" << client_id << "---\n";
//     for(auto it_topic = _subscription_map.begin(); it_topic != _subscription_map.end(); it_topic++)
//     {
//         for(auto it_client = it_topic->second.begin(); it_client != it_topic->second.end(); it_client++)
//         {
//             if(it_client->client_id == client_id)
//             {   
//                 std::cout << it_topic->first << '(' << (int)it_client->qos << ")\n";
//                 break;
//             }
//         }
//     }
//     std::cout << '\n';
// }

// // Return specific subscription.
// subscription subscription_manager::get_subscribtion(const std::string& topic, const std::string& client_id)
// {
//     std::vector<subscription> sub = _subscription_map[topic];
    
//     auto it = std::find_if(sub.begin(), sub.end(), [&](const subscription s)
//     {
//         return s.client_id == client_id;
//     });

//     return *it;
// }