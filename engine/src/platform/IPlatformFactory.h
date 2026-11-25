#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

#include <memory>

namespace engine::platform
{
    // Maakt de platformonderdelen van één backend (SDL3 of Headless).
    // De composition root kiest welke factory er gebruikt wordt; verder kent niemand de backend.
    //
    // Eigenaarschap en levensduur:
    // - Alles komt terug als unique_ptr: de aanroeper bezit het resultaat.
    // - De factory moet langer leven dan alles wat hij heeft aangemaakt
    //   (de SDL3-factory roept in zijn destructor SDL_Quit() aan).
    // - Een create...(IWindow&) accepteert alleen een window dat door DEZE factory is gemaakt.
    //
    // Uitbreiden: de eigenaar van een onderdeel voegt hier zijn eigen create...() toe
    // (createInputSource, createRenderer, createAudioBackend, ...) en implementeert die
    // in zowel SDL3PlatformFactory als HeadlessPlatformFactory.
    class IPlatformFactory
    {
    public:
        IPlatformFactory() = default;
        virtual ~IPlatformFactory() = default;

        IPlatformFactory(const IPlatformFactory&) = delete;
        auto operator=(const IPlatformFactory&) -> IPlatformFactory& = delete;
        IPlatformFactory(IPlatformFactory&&) = delete;
        auto operator=(IPlatformFactory&&) -> IPlatformFactory& = delete;

        [[nodiscard]] virtual std::unique_ptr<IWindow> createWindow(const WindowConfig& config) = 0;
    };
} // namespace engine::platform
