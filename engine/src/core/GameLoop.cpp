#include "core/GameLoop.h"

#include "facades/Time.h"
#include "runtime/Scene.h"
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

#include <algorithm>
#include <iostream>
#include <thread>

namespace engine
{
    namespace
    {
        constexpr float maxFrameTime{0.25F};
    } // namespace

    GameLoop::GameLoop(SceneManager& sceneManager,
                       subsystems::InputSubsystem& input,
                       subsystems::PhysicsSubsystem& physics,
                       subsystems::AnimationSubsystem& animation,
                       subsystems::AudioSubsystem& audio,
                       subsystems::RenderSubsystem& render,
                       subsystems::AISubsystem& ai,
                       subsystems::UISubsystem& ui,
                       subsystems::DebugSubsystem& debug,
                       subsystems::LevelEditor& editor,
                       int targetFrameRate)
        : sceneManager(sceneManager)
        , input(input)
        , physics(physics)
        , animation(animation)
        , audio(audio)
        , render(render)
        , ai(ai)
        , ui(ui)
        , debug(debug)
        , editor(editor)
        , targetFrameRate(std::max(0, targetFrameRate))
    {
    }

    void GameLoop::run()
    {
        lastFrameTime = Clock::now();

        while (!quitRequested)
            tick();
    }

    void GameLoop::requestQuit()
    {
        quitRequested = true;
    }

    void GameLoop::tick()
    {
        // 1. Tijd
        const Clock::time_point frameStart = Clock::now();
        const float frameTime = std::chrono::duration<float>(frameStart - lastFrameTime).count();
        lastFrameTime = frameStart;

        Time::advance(std::min(frameTime, maxFrameTime));
        const float deltaTime = Time::getDeltaTime();

        // 2. Input
        input.update();
        if (input.isQuitRequested())
        {
            requestQuit();
            return;
        }

        Scene* scene = sceneManager.getActiveScene();

        // 3-5. Simulatie (Unity-volgorde: physics voor de scripts)
        if (scene != nullptr && !editor.isEditing())
        {
            physics.update(deltaTime, *scene);
            scene->update();
            ai.update(deltaTime, *scene);
            animation.update(deltaTime, *scene);
            audio.update(deltaTime, *scene);
        }

        // 6. Editor, UI en debug
        editor.update();
        ui.update(deltaTime);
        debug.update(Time::getUnscaledDeltaTime());

        // 7. Render
        render.beginFrame();
        if (scene != nullptr)
            render.render(*scene);
        debug.render();
        ui.render();
        render.endFrame();

        // 8. Opruimen
        sceneManager.processPendingDestruction();

        logFrameStats();

        // 9. Framerate-limiet (0 = onbeperkt).
        if (targetFrameRate > 0)
        {
            const std::chrono::duration<double> frameDuration{1.0 / targetFrameRate};
            std::this_thread::sleep_until(frameStart + std::chrono::duration_cast<Clock::duration>(frameDuration));
        }
    }

    void GameLoop::logFrameStats()
    {
        statsElapsed += Time::getUnscaledDeltaTime();
        ++statsFrames;

        if (statsElapsed >= 1.0F)
        {
            std::cout << "[GameLoop] " << statsFrames << " fps, dt=" << (statsElapsed / static_cast<float>(statsFrames))
                      << " s\n";
            statsElapsed = 0.0F;
            statsFrames = 0;
        }
    }
} // namespace engine
