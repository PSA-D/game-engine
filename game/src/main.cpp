#include "Engine.h"
#include "PlayerInput.h"
#include "src/backends/sdl3/SDL3InputSource.h"
#include "src/input/InputSubsystem.h"

#include <cstdlib>
#include <iostream>

auto main() -> int
{
    engine::Engine gameEngine;
    gameEngine.init();

    engine::backends::SDL3Window window{};
    engine::backends::SDL3InputSource source{window};
    engine::subsystems::InputSubsystem inputSubsystem{source};
    PlayerInput playerInput{};
    inputSubsystem.addKeyListener(&playerInput);

    while (gameEngine.isRunning())
    {
        inputSubsystem.update(0.1f);

        // Game loop placeholder
        std::cout << "Engine is running\n";
        gameEngine.shutdown();
    }

    return EXIT_SUCCESS;
}
