#pragma once

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <queue>
#include <nlohmann/json.hpp>
#include "ToolBase.h"

namespace Core {

    /**
     * @brief Système de communication pub/sub entre les tools
     *
     * L'EventBus permet aux tools de communiquer entre eux sans couplage direct.
     *
     * Workflow:
     * 1. Un tool publie un événement: eventBus->Publish("dice.rolled", {{"result", 15}})
     * 2. L'événement est ajouté à une queue
     * 3. À la fin du frame, tous les événements sont distribués aux abonnés
     * 4. Les callbacks des tools abonnés sont appelés
     *
     * Exemple d'usage:
     * @code
     * // Dans DiceRollerTool::OnCreate()
     * eventBus->Subscribe("dice.roll_request", [this](const Event& e) {
     *     int result = RollDice(e.data["formula"]);
     *     eventBus->Publish("dice.rolled", {{"result", result}});
     * });
     *
     * // Dans CombatTool::OnCreate()
     * eventBus->Subscribe("dice.rolled", [this](const Event& e) {
     *     ApplyDamage(e.data["result"]);
     * });
     * @endcode
     */
    class EventBus {
    public:
        using EventCallback = std::function<void(const Event&)>;
        using SubscriptionID = int;

        EventBus();
        ~EventBus();

        /**
         * @brief Publie un événement
         *
         * L'événement est ajouté à une queue et sera distribué lors du prochain ProcessEvents().
         *
         * @param eventType Type de l'événement (ex: "dice.rolled")
         * @param data Données JSON associées
         * @param sourceInstanceID ID de l'instance qui émet l'événement
         */
        void Publish(const std::string& eventType,
                    const nlohmann::json& data,
                    const std::string& sourceInstanceID = "");

        /**
         * @brief S'abonne à un type d'événement
         *
         * @param eventType Type d'événement à écouter (ex: "dice.rolled")
         * @param callback Fonction appelée quand l'événement est émis
         * @return ID de l'abonnement (pour se désabonner plus tard)
         */
        SubscriptionID Subscribe(const std::string& eventType, EventCallback callback);

        /**
         * @brief Se désabonne d'un événement
         *
         * @param subscriptionID ID retourné par Subscribe()
         */
        void Unsubscribe(SubscriptionID subscriptionID);

        /**
         * @brief Se désabonne de tous les événements d'un type
         *
         * @param eventType Type d'événement
         */
        void UnsubscribeAll(const std::string& eventType);

        /**
         * @brief Traite tous les événements en attente
         *
         * Appelé une fois par frame par l'Application.
         * Distribue tous les événements de la queue aux abonnés.
         */
        void ProcessEvents();

        /**
         * @brief Retourne le nombre d'événements en attente
         */
        size_t GetQueueSize() const { return eventQueue.size(); }

        /**
         * @brief Vide la queue d'événements (utile pour les tests)
         */
        void ClearQueue() { eventQueue = std::queue<Event>(); }

        /**
         * @brief Active/désactive le mode debug (affiche tous les événements)
         */
        void SetDebugMode(bool enabled) { debugMode = enabled; }

    private:
        struct Subscription {
            SubscriptionID id;
            std::string eventType;
            EventCallback callback;
        };

        std::map<std::string, std::vector<Subscription>> subscriptions; // eventType -> abonnés
        std::queue<Event> eventQueue;                                   // Queue d'événements
        SubscriptionID nextSubscriptionID = 0;                          // Compteur pour IDs uniques
        bool debugMode = false;                                         // Mode debug

        // Helper pour trouver un abonnement par ID
        bool FindSubscription(SubscriptionID id, std::string& outEventType, size_t& outIndex);
    };

} // namespace Core
