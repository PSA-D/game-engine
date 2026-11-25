#pragma once

#include "utils/Size.h"

#include <cstdint>
#include <string>

namespace engine
{
    // Welke platform-backend de engine gebruikt voor venster, input en (later) renderer en audio.
    enum class PlatformBackend : std::uint8_t
    {
        SDL3,
        Headless,
    };

    enum class PhysicsBackend : std::uint8_t
    {
        Box2D,
        None,
    };

    enum class UIBackend : std::uint8_t
    {
        RmlUi,
        None,
    };

    enum class DebugBackend : std::uint8_t
    {
        ImGui,
        None,
    };

    // Alles wat de game bij het opstarten aan de engine meegeeft.
    // De composition root (EngineImpl) leidt hier de configuratie van de onderdelen uit af,
    // zoals de WindowConfig voor het venster.
    struct EngineConfig
    {
        static constexpr Size defaultWindowSize{1280, 720};

        std::string appName{"Game"};        // wordt ook de venstertitel
        Size windowSize{defaultWindowSize}; // in schermeenheden
        bool fullscreen{false};
        bool resizable{true};
        bool vsync{true};

        PlatformBackend platformBackend{PlatformBackend::SDL3};
        PhysicsBackend physicsBackend{PhysicsBackend::None};
        UIBackend uiBackend{UIBackend::None};
        DebugBackend debugBackend{DebugBackend::None};
    };
} // namespace engine
