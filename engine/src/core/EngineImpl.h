#pragma once

#include "core/EngineConfig.h"
#include "core/GameLoop.h"
#include "platform/IInputSource.h"
#include "platform/IPlatformFactory.h"
#include "platform/IWindow.h"
#include "runtime/SceneManager.h"
#include "subsystems/ai/AISubsystem.h"
#include "subsystems/animation/AnimationSubsystem.h"
#include "subsystems/audio/AudioSubsystem.h"
#include "subsystems/debug/DebugSubsystem.h"
#include "subsystems/editor/LevelEditor.h"
#include "subsystems/input/InputSubsystem.h"
#include "subsystems/physics/PhysicsSubsystem.h"
#include "subsystems/render/RenderSubsystem.h"
#include "subsystems/ui/UISubsystem.h"

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
        std::unique_ptr<platform::IInputSource> inputSource;
        // audioBackend, paths komen hier

        // 2. Subsystemen, in de volgorde van het KD
        // assets komt hier
        subsystems::InputSubsystem input;
        subsystems::PhysicsSubsystem physics;
        subsystems::RenderSubsystem render;
        subsystems::AudioSubsystem audio;
        subsystems::AnimationSubsystem animation;
        subsystems::UISubsystem ui;
        subsystems::AISubsystem ai;
        // persistence komt hier
        subsystems::LevelEditor editor;
        subsystems::DebugSubsystem debug;

        // 3. SceneManager + GameLoop
        SceneManager sceneManager; // na merge Dirk: eerst EventBus eventBus; dan sceneManager{eventBus}
        GameLoop gameLoop;
    };
} // namespace engine
