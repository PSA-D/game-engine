#include "Engine.h"
#include "LoadingScene.h"
#include "Scene.h"
#include "SceneManager.h"

#include <cstdlib>
#include <iostream>

namespace
{
constexpr int loadingSceneId = 99;
}

auto main() -> int
{
    engine::Engine gameEngine;
    gameEngine.init();

    engine::EventBus eventBus;
    engine::SceneManager sceneManager(eventBus);

    // Repeated load / leave / reactivate across several cycles.
    for (int cycle = 0; cycle < 3; ++cycle)
    {
        // Fresh scenes each cycle so references never dangle after unload.
        engine::Scene& menu = sceneManager.createScene("Menu");
        engine::Scene& gameplay = sceneManager.createScene("Gameplay");

        // Load the menu scene.
        sceneManager.activateScene(menu);
        std::cout << "Active scene: " << sceneManager.getActiveScene()->getName() << "\n";

        // Show a loading screen (load, then leave).
        {
            engine::LoadingScene loading(loadingSceneId + cycle, "Loading");
            loading.init();
            std::cout << "Active scene: " << loading.getName() << " (loading)\n";
            loading.shutdown();
        }

        // Enter the gameplay scene.
        sceneManager.activateScene(gameplay);
        std::cout << "Active scene: " << sceneManager.getActiveScene()->getName() << "\n";

        // Leave both scenes.
        sceneManager.unloadScene(gameplay);
        sceneManager.unloadScene(menu);
        std::cout << "No active scene\n";
    }

    gameEngine.shutdown();
    return EXIT_SUCCESS;
}
