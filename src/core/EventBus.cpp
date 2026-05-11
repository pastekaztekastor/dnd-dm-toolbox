#include "../../include/core/EventBus.h"
#include <iostream>
#include <chrono>

namespace Core {

EventBus::EventBus() {
    if (debugMode) {
        std::cout << "EventBus: Initialized" << std::endl;
    }
}

EventBus::~EventBus() {
    subscriptions.clear();
    if (debugMode) {
        std::cout << "EventBus: Destroyed" << std::endl;
    }
}

void EventBus::Publish(const std::string& eventType,
                       const nlohmann::json& data,
                       const std::string& sourceInstanceID) {
    Event event;
    event.type = eventType;
    event.data = data;
    event.sourceInstanceID = sourceInstanceID;

    // Timestamp Unix en secondes
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    event.timestamp = std::chrono::duration<double>(duration).count();

    eventQueue.push(event);

    if (debugMode) {
        std::cout << "EventBus: Published event '" << eventType
                  << "' from " << (sourceInstanceID.empty() ? "system" : sourceInstanceID)
                  << " (queue size: " << eventQueue.size() << ")" << std::endl;
    }
}

EventBus::SubscriptionID EventBus::Subscribe(const std::string& eventType, EventCallback callback) {
    Subscription sub;
    sub.id = nextSubscriptionID++;
    sub.eventType = eventType;
    sub.callback = callback;

    subscriptions[eventType].push_back(sub);

    if (debugMode) {
        std::cout << "EventBus: Subscribed to '" << eventType
                  << "' (ID: " << sub.id << ", "
                  << subscriptions[eventType].size() << " subscribers)" << std::endl;
    }

    return sub.id;
}

void EventBus::Unsubscribe(SubscriptionID subscriptionID) {
    std::string eventType;
    size_t index;

    if (FindSubscription(subscriptionID, eventType, index)) {
        subscriptions[eventType].erase(subscriptions[eventType].begin() + index);

        if (debugMode) {
            std::cout << "EventBus: Unsubscribed ID " << subscriptionID
                      << " from '" << eventType << "'" << std::endl;
        }

        // Nettoyer la map si plus d'abonnés
        if (subscriptions[eventType].empty()) {
            subscriptions.erase(eventType);
        }
    }
}

void EventBus::UnsubscribeAll(const std::string& eventType) {
    auto it = subscriptions.find(eventType);
    if (it != subscriptions.end()) {
        size_t count = it->second.size();
        subscriptions.erase(it);

        if (debugMode) {
            std::cout << "EventBus: Unsubscribed all (" << count
                      << " subscribers) from '" << eventType << "'" << std::endl;
        }
    }
}

void EventBus::ProcessEvents() {
    if (eventQueue.empty()) {
        return;
    }

    if (debugMode) {
        std::cout << "EventBus: Processing " << eventQueue.size() << " events" << std::endl;
    }

    // Traiter tous les événements de la queue
    while (!eventQueue.empty()) {
        Event event = eventQueue.front();
        eventQueue.pop();

        // Trouver les abonnés pour ce type d'événement
        auto it = subscriptions.find(event.type);
        if (it != subscriptions.end()) {
            if (debugMode) {
                std::cout << "EventBus: Dispatching '" << event.type
                          << "' to " << it->second.size() << " subscribers" << std::endl;
            }

            // Appeler tous les callbacks
            for (const auto& sub : it->second) {
                try {
                    sub.callback(event);
                } catch (const std::exception& e) {
                    std::cerr << "EventBus: Error in callback for '" << event.type
                              << "': " << e.what() << std::endl;
                }
            }
        } else if (debugMode) {
            std::cout << "EventBus: No subscribers for '" << event.type << "'" << std::endl;
        }
    }
}

bool EventBus::FindSubscription(SubscriptionID id, std::string& outEventType, size_t& outIndex) {
    for (auto& [eventType, subs] : subscriptions) {
        for (size_t i = 0; i < subs.size(); ++i) {
            if (subs[i].id == id) {
                outEventType = eventType;
                outIndex = i;
                return true;
            }
        }
    }
    return false;
}

} // namespace Core
