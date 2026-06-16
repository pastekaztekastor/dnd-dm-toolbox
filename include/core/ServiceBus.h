#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace Core {

    using ServiceHandler = std::function<nlohmann::json(const nlohmann::json&)>;
    using ServiceID      = int;

    /**
     * @brief Registre synchrone de services inter-plugins (request/response)
     *
     * Complément à l'EventBus (async, one-way) pour les cas où un plugin
     * a besoin d'une réponse immédiate d'un autre plugin.
     *
     * Deux modes :
     * - CallService  : interroge le premier provider enregistré (1-to-1)
     * - QueryService : interroge tous les providers enregistrés (1-to-N)
     *
     * Exemple d'usage:
     * @code
     * // Provider (dans race_dd_5_5::OnCreate)
     * RegisterService("race.getAll", [this](const nlohmann::json&) {
     *     return nlohmann::json{ {"races", buildRaceList()} };
     * });
     *
     * // Consommateur (dans character_creator)
     * auto result = CallService("race.getAll", {});
     * if (result) { auto races = (*result)["races"]; }
     * @endcode
     */
    class ServiceBus {
    public:
        ServiceBus();
        ~ServiceBus();

        /**
         * @brief Enregistre un handler pour un service nommé
         *
         * Plusieurs handlers peuvent être enregistrés sous le même nom
         * (QueryService les appelle tous, CallService prend le premier).
         *
         * @param name  Nom du service (ex: "race.getAll")
         * @param handler Fonction (json params) → json result
         * @return ID d'enregistrement (conserver pour UnregisterService)
         */
        ServiceID RegisterService(const std::string& name, ServiceHandler handler);

        /**
         * @brief Retire un handler précédemment enregistré
         *
         * @param id ID retourné par RegisterService
         */
        void UnregisterService(ServiceID id);

        /**
         * @brief Appelle le premier handler enregistré pour ce service
         *
         * @param name   Nom du service
         * @param params Paramètres JSON
         * @return Résultat JSON, ou nullopt si service absent ou exception
         */
        std::optional<nlohmann::json> CallService(const std::string& name,
                                                  const nlohmann::json& params);

        /**
         * @brief Appelle tous les handlers enregistrés pour ce service
         *
         * Utile pour les requêtes broadcast ("qui peut fournir X ?").
         * Les handlers qui lèvent une exception sont ignorés individuellement.
         *
         * @param name   Nom du service
         * @param params Paramètres JSON
         * @return Vecteur de résultats (vide si aucun provider)
         */
        std::vector<nlohmann::json> QueryService(const std::string& name,
                                                 const nlohmann::json& params);

        /**
         * @brief Retourne true si au moins un handler est enregistré sous ce nom
         */
        bool HasService(const std::string& name) const;

        /**
         * @brief Retourne les noms de tous les services actuellement enregistrés
         */
        std::vector<std::string> GetServiceNames() const;

        /**
         * @brief Active/désactive le mode debug (affiche les appels sur stdout)
         */
        void SetDebugMode(bool enabled) { debugMode = enabled; }

    private:
        struct Registration {
            ServiceID      id;
            std::string    name;
            ServiceHandler handler;
        };

        std::unordered_map<std::string, std::vector<Registration>> services;
        ServiceID nextServiceID = 0;
        bool      debugMode     = false;

        bool FindRegistration(ServiceID id, std::string& outName, size_t& outIndex);
    };

} // namespace Core
