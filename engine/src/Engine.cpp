#include "Engine.h"

#include <iostream>

namespace engine
{
Engine::Engine() = default;

Engine::~Engine()
{
    shutdown();
}

void Engine::init()
{
    std::cout << "Engine initialized\n";
    running = true;
}

void Engine::shutdown()
{
    if (!running)
        return;

    running = false;
    std::cout << "Engine shut down\n";
}

bool Engine::isRunning() const
{
    return running;
}
} // namespace engine
