#pragma once

#include <chrono>

namespace engine
{
    class SceneManager;
} // namespace engine

namespace engine::subsystems
{
    class InputSubsystem;
    class PhysicsSubsystem;
    class AnimationSubsystem;
    class AudioSubsystem;
    class RenderSubsystem;
    class AISubsystem;
    class UISubsystem;
    class DebugSubsystem;
    class LevelEditor;
} // namespace engine::subsystems

namespace engine
{
    class GameLoop
    {
    public:
        GameLoop(SceneManager& sceneManager,
                 subsystems::InputSubsystem& input,
                 subsystems::PhysicsSubsystem& physics,
                 subsystems::AnimationSubsystem& animation,
                 subsystems::AudioSubsystem& audio,
                 subsystems::RenderSubsystem& render,
                 subsystems::AISubsystem& ai,
                 subsystems::UISubsystem& ui,
                 subsystems::DebugSubsystem& debug,
                 subsystems::LevelEditor& editor,
                 int targetFrameRate);

        GameLoop(const GameLoop&) = delete;
        auto operator=(const GameLoop&) -> GameLoop& = delete;
        GameLoop(GameLoop&&) = delete;
        auto operator=(GameLoop&&) -> GameLoop& = delete;
        ~GameLoop() = default;

        void run();

        void requestQuit();

    private:
        using Clock = std::chrono::steady_clock;

        void tick();
        void logFrameStats(); // TIJDELIJK, tot het DebugSubsystem (FpsCounter) er is

        SceneManager& sceneManager;
        subsystems::InputSubsystem& input;
        subsystems::PhysicsSubsystem& physics;
        subsystems::AnimationSubsystem& animation;
        subsystems::AudioSubsystem& audio;
        subsystems::RenderSubsystem& render;
        subsystems::AISubsystem& ai;
        subsystems::UISubsystem& ui;
        subsystems::DebugSubsystem& debug;
        subsystems::LevelEditor& editor;

        int targetFrameRate;
        Clock::time_point lastFrameTime{Clock::now()};
        bool quitRequested{false};

        float statsElapsed{0.0F};
        int statsFrames{0};
    };
} // namespace engine
