#include "CompositionRoot.h"

#include "backends/sdl3/SDL3PlatformFactory.h"
#include "core/EngineConfig.h"
#include "platform/headless/HeadlessPlatformFactory.h"

#include <stdexcept>

namespace engine::core
{
    platform::WindowConfig makeWindowConfig(const EngineConfig& config)
    {
        platform::WindowConfig windowConfig;
        windowConfig.title = config.appName;
        windowConfig.size = config.windowSize;
        windowConfig.fullscreen = config.fullscreen;
        windowConfig.resizable = config.resizable;
        windowConfig.vsync = config.vsync;
        return windowConfig;
    }

    std::unique_ptr<platform::IPlatformFactory> createPlatformFactory(PlatformBackend backend)
    {
        switch (backend)
        {
        case PlatformBackend::SDL3:
            return std::make_unique<backends::SDL3PlatformFactory>();
        case PlatformBackend::Headless:
            return std::make_unique<platform::HeadlessPlatformFactory>();
        }

        throw std::invalid_argument("Onbekende PlatformBackend");
    }
} // namespace engine::core
