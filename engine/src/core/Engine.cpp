#include "Engine.h"

#include "core/EngineImpl.h"

namespace engine
{
    Engine::Engine(const EngineConfig& config)
        : impl(std::make_unique<EngineImpl>(config))
    {
    }

    Engine::~Engine() = default;

    void Engine::run()
    {
        impl->run();
    }
} // namespace engine
