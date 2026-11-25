#include "Engine.h"

#include "Scene.h"

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
    {
        return;
    }

    running = false;
    std::cout << "Engine shut down\n";
}

void Engine::run()
{
    while (isRunning())
    {
        if (activeScene_ != nullptr)
        {
            activeScene_->update();
            activeScene_->render();
        }
    }
}

auto Engine::isRunning() const noexcept -> bool
{
    return running;
}

auto Engine::setActiveScene(Scene& scene) -> void
{
    activeScene_ = &scene;
}
} // namespace engine
