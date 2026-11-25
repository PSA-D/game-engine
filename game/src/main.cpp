#include "Engine.h"

#include <cstdlib>
#include <iostream>

auto main() -> int
{
    engine::Engine gameEngine;
    gameEngine.init();

    while (gameEngine.isRunning())
    {
        // Game loop placeholder
        std::cout << "Engine is running\n";
        gameEngine.shutdown();
    }

    return EXIT_SUCCESS;
}
