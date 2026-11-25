#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
    // Instellingen voor het aanmaken van een venster. Pure data.
    // Wordt door de composition root opgesteld uit de EngineConfig.
    struct WindowConfig
    {
        static constexpr Size defaultSize{1280, 720};

        std::string title{"Game"};
        Size size{defaultSize}; // in schermeenheden, niet in pixels
        bool fullscreen{false};
        bool resizable{true};
        bool vsync{true}; // wordt toegepast door de renderer, zodra die er is
    };
} // namespace engine::platform
