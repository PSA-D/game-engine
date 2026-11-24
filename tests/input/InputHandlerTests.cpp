#include "input/InputHandler.h"
#include "input/InputState.h"
#include "input/InputSubsystem.h"
#include "input/commands/FunctionCommand.h"
#include "platform/Events.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace engine::subsystems;
using namespace engine::platform;
using namespace engine;

TEST_CASE("InputHandler binds and triggers key commands correctly", "[input][handler]")
{
    InputHandler handler;
    InputState state{};

    int pressedCount = 0;
    int releasedCount = 0;
    int heldCount = 0;

    handler.bindKey(
        KeyCode::Space, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&]() { ++pressedCount; }));
    handler.bindKey(
        KeyCode::Space, InputTrigger::Released, std::make_unique<FunctionCommand>([&]() { ++releasedCount; }));
    handler.bindKey(KeyCode::Space, InputTrigger::Held, std::make_unique<FunctionCommand>([&]() { ++heldCount; }));

    const auto spaceIdx = static_cast<std::size_t>(KeyCode::Space);

    SECTION("Pressed trigger fires only when keysPressed is true")
    {
        state.keysPressed.at(spaceIdx) = true;
        handler.handleInput(state);

        REQUIRE(pressedCount == 1);
        REQUIRE(releasedCount == 0);
        REQUIRE(heldCount == 0);
    }

    SECTION("Released trigger fires only when keysReleased is true")
    {
        state.keysReleased.at(spaceIdx) = true;
        handler.handleInput(state);

        REQUIRE(pressedCount == 0);
        REQUIRE(releasedCount == 1);
        REQUIRE(heldCount == 0);
    }

    SECTION("Held trigger fires only when keysDown is true")
    {
        state.keysDown.at(spaceIdx) = true;
        handler.handleInput(state);

        REQUIRE(pressedCount == 0);
        REQUIRE(releasedCount == 0);
        REQUIRE(heldCount == 1);
    }

    SECTION("Unbound key does not execute")
    {
        handler.unbindKey(KeyCode::Space, InputTrigger::Pressed);
        state.keysPressed.at(spaceIdx) = true;
        handler.handleInput(state);

        REQUIRE(pressedCount == 0);
    }
}

TEST_CASE("InputHandler binds, unbinds, and clears mouse button commands", "[input][handler]")
{
    InputHandler handler;
    InputState state{};

    int leftClickCount = 0;
    int rightHoldCount = 0;

    handler.bindMouseButton(
        MouseButton::Left, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&]() { ++leftClickCount; }));
    handler.bindMouseButton(
        MouseButton::Right, InputTrigger::Held, std::make_unique<FunctionCommand>([&]() { ++rightHoldCount; }));

    const auto leftIdx = static_cast<std::size_t>(MouseButton::Left);
    const auto rightIdx = static_cast<std::size_t>(MouseButton::Right);

    state.mouseButtonsPressed.at(leftIdx) = true;
    state.mouseButtonsDown.at(rightIdx) = true;
    handler.handleInput(state);

    REQUIRE(leftClickCount == 1);
    REQUIRE(rightHoldCount == 1);

    // Unbind only Left
    handler.unbindMouseButton(MouseButton::Left, InputTrigger::Pressed);
    handler.handleInput(state);

    REQUIRE(leftClickCount == 1);
    REQUIRE(rightHoldCount == 2);

    // Clear all remaining bindings
    handler.clear();
    handler.handleInput(state);

    REQUIRE(leftClickCount == 1);
    REQUIRE(rightHoldCount == 2);
}

