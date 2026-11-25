#pragma once

#include "EngineConfig.h"
#include "platform/IPlatformFactory.h"
#include "platform/IWindow.h"

#include <memory>

namespace engine
{
    // De composition root: maakt alle onderdelen van de engine aan en koppelt ze.
    // Dit is de enige plek die alle backends en subsystemen kent.
    class EngineImpl
    {
    public:
        explicit EngineImpl(const EngineConfig& config);
        ~EngineImpl();

        EngineImpl(const EngineImpl&) = delete;
        auto operator=(const EngineImpl&) -> EngineImpl& = delete;
        EngineImpl(EngineImpl&&) = delete;
        auto operator=(EngineImpl&&) -> EngineImpl& = delete;

        void run();

    private:
        // ┌──────────────────────────────────────────────────────────────────────┐
        // │ REGEL: volgorde van declareren = volgorde van aanmaken.              │
        // │ C++ vernietigt members in OMGEKEERDE volgorde, dus afsluiten gaat    │
        // │ vanzelf goed. Zet een nieuw onderdeel altijd ONDER alles waar het    │
        // │ van afhangt, en voeg het op dezelfde plek toe in de init-lijst.      │
        // └──────────────────────────────────────────────────────────────────────┘

        // 1. Platform
        std::unique_ptr<platform::IPlatformFactory> factory;
        std::unique_ptr<platform::IWindow> window;
        // renderer komt HIER (na window, voor inputSource)
        // inputSource komt HIER (stap 8, na merge van feat/input-source)
        // audioBackend, paths komen hier

        // 2. Subsystemen
        // assets, input, physics, render, audio, animation, ui, ai, persistence, editor, debug

        // 3. SceneManager + GameLoop
    };
} // namespace engine
