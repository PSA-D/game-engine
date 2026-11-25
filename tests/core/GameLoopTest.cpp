#include "core/GameLoop.h"

#include "SceneManager.h"
#include "subsystems/ai/AISubsystem.h"
#include "subsystems/animation/AnimationSubsystem.h"
#include "subsystems/audio/AudioSubsystem.h"
#include "subsystems/debug/DebugSubsystem.h"
#include "subsystems/editor/LevelEditor.h"
#include "subsystems/input/InputSubsystem.h"
#include "subsystems/physics/PhysicsSubsystem.h"
#include "subsystems/render/RenderSubsystem.h"
#include "subsystems/ui/UISubsystem.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    TEST_CASE("GameLoop::run() stopt na requestQuit()", "[unit][core]")
    {
        SceneManager sceneManager;
        subsystems::InputSubsystem input;
        subsystems::PhysicsSubsystem physics;
        subsystems::AnimationSubsystem animation;
        subsystems::AudioSubsystem audio;
        subsystems::RenderSubsystem render;
        subsystems::AISubsystem ai;
        subsystems::UISubsystem ui;
        subsystems::DebugSubsystem debug;
        subsystems::LevelEditor editor;

        GameLoop loop{sceneManager, input, physics, animation, audio, render, ai, ui, debug, editor, 60};

        loop.requestQuit();
        loop.run(); // mag niet blijven hangen

        SUCCEED("run() keerde terug");
    }
} // namespace engine
