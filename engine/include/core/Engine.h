#pragma once

#include "EngineConfig.h"

#include <memory>

namespace engine
{
    class EngineImpl;

    // De enige ingang voor de game.
    // - De constructor zet via de composition root (EngineImpl) alle onderdelen op.
    // - run() draait de game tot hij stopt.
    // - De destructor ruimt alles op in omgekeerde volgorde.
    // Backends en subsystemen zitten verborgen achter EngineImpl (pimpl), zodat de game ze niet kent.
    class Engine
    {
    public:
        explicit Engine(const EngineConfig& config);
        ~Engine();

        Engine(const Engine&) = delete;
        auto operator=(const Engine&) -> Engine& = delete;
        Engine(Engine&&) = delete;
        auto operator=(Engine&&) -> Engine& = delete;

        void run();

    private:
        std::unique_ptr<EngineImpl> impl;
    };
} // namespace engine
