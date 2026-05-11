#include "../include/core/Application.h"
#include <iostream>
#include <exception>

/**
 * @brief Point d'entrée de l'application D&D DM Toolbox
 *
 * Cette nouvelle version utilise l'architecture plugin-based.
 */
int main(int argc, char** argv) {
    std::cout << R"(
    ╔═══════════════════════════════════════════════╗
    ║                                               ║
    ║         D&D DM TOOLBOX - Version 2.0          ║
    ║        Architecture Plugin-Based              ║
    ║                                               ║
    ╚═══════════════════════════════════════════════╝
    )" << std::endl;

    try {
        // Créer l'application
        Core::Application app;

        // Initialiser
        if (!app.Init()) {
            std::cerr << "❌ Erreur: Échec de l'initialisation" << std::endl;
            return 1;
        }

        // Lancer la boucle principale
        app.Run();

        // Arrêter proprement
        app.Shutdown();

        std::cout << "\n👋 À bientôt!" << std::endl;

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ Exception non gérée: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Exception inconnue" << std::endl;
        return 1;
    }
}
