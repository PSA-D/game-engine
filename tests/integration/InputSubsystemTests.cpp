#include "input/InputHandler.h"
#include "input/InputState.h"
#include "input/commands/FunctionCommand.h"
#include "platform/Events.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <variant>

using namespace engine::subsystems;
using namespace engine::platform;
using namespace engine;

namespace
{
    // Helper that applies a custom PlatformEvent to an InputState
    void applyPlatformEvent(InputState& state, const PlatformEvent& event)
    {
        if (const auto* keyEvent = std::get_if<KeyEvent>(&event))
        {
            const auto idx = static_cast<std::size_t>(keyEvent->key);
            if (idx < state.keysDown.size())
            {
                if (keyEvent->pressed)
                {
                    if (!state.keysDown.at(idx))
                        state.keysPressed.at(idx) = true;
                    state.keysDown.at(idx) = true;
                }
                else
                {
                    state.keysDown.at(idx) = false;
                    state.keysReleased.at(idx) = true;
                }
            }
        }
        else if (const auto* mouseEvent = std::get_if<MouseButtonEvent>(&event))
        {
            const auto idx = static_cast<std::size_t>(mouseEvent->button);
            if (idx < state.mouseButtonsDown.size())
            {
                if (mouseEvent->pressed)
                {
                    if (!state.mouseButtonsDown.at(idx))
                        state.mouseButtonsPressed.at(idx) = true;
                    state.mouseButtonsDown.at(idx) = true;
                }
                else
                {
                    state.mouseButtonsDown.at(idx) = false;
                    state.mouseButtonsReleased.at(idx) = true;
                }
            }
        }
    }

    void resetFrameState(InputState& state)
    {
        state.keysPressed.fill(false);
        state.keysReleased.fill(false);
        state.mouseButtonsPressed.fill(false);
        state.mouseButtonsReleased.fill(false);
    }
} // namespace

TEST_CASE("InputHandler binds and executes key commands from PlatformEvents", "[input][handler]")
{
    InputHandler handler;
    InputState state{};

    int jumpPressedCount = 0;
    int jumpHeldCount = 0;
    int jumpReleasedCount = 0;

    handler.bindKey(
        KeyCode::Space, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&] { ++jumpPressedCount; }));
    handler.bindKey(KeyCode::Space, InputTrigger::Held, std::make_unique<FunctionCommand>([&] { ++jumpHeldCount; }));
    handler.bindKey(
        KeyCode::Space, InputTrigger::Released, std::make_unique<FunctionCommand>([&] { ++jumpReleasedCount; }));

    SECTION("Key press, hold across frames, and release trigger the matching commands")
    {
        // Frame 1: Custom KeyEvent (Space pressed)
        resetFrameState(state);
        applyPlatformEvent(state, PlatformEvent{KeyEvent{.key = KeyCode::Space, .mods = {}, .pressed = true, .repeat = false}});
        handler.handleInput(state);

        REQUIRE(jumpPressedCount == 1);
        REQUIRE(jumpHeldCount == 1);
        REQUIRE(jumpReleasedCount == 0);

        // Frame 2: No new events -> Pressed resets, Held continues firing
        resetFrameState(state);
        handler.handleInput(state);

        REQUIRE(jumpPressedCount == 1);
        REQUIRE(jumpHeldCount == 2);
        REQUIRE(jumpReleasedCount == 0);

        // Frame 3: Custom KeyEvent (Space released)
        resetFrameState(state);
        applyPlatformEvent(state, PlatformEvent{KeyEvent{.key = KeyCode::Space, .mods = {}, .pressed = false, .repeat = false}});
        handler.handleInput(state);

        REQUIRE(jumpPressedCount == 1);
        REQUIRE(jumpHeldCount == 2);
        REQUIRE(jumpReleasedCount == 1);
    }

    SECTION("Unbound key does not trigger command")
    {
        handler.unbindKey(KeyCode::Space, InputTrigger::Pressed);

        resetFrameState(state);
        applyPlatformEvent(state, PlatformEvent{KeyEvent{.key = KeyCode::Space, .mods = {}, .pressed = true, .repeat = false}});
        handler.handleInput(state);

        REQUIRE(jumpPressedCount == 0);
        REQUIRE(jumpHeldCount == 1);
    }
}

TEST_CASE("InputHandler binds, unbinds, and clears mouse button commands from PlatformEvents", "[input][handler]")
{
    InputHandler handler;
    InputState state{};

    int leftClickCount = 0;
    int rightReleaseCount = 0;

    handler.bindMouseButton(
        MouseButton::Left, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&] { ++leftClickCount; }));
    handler.bindMouseButton(
        MouseButton::Right, InputTrigger::Released, std::make_unique<FunctionCommand>([&] { ++rightReleaseCount; }));

    // Frame 1: Left pressed, Right pressed
    resetFrameState(state);
    applyPlatformEvent(state, PlatformEvent{MouseButtonEvent{.button = MouseButton::Left, .pressed = true}});
    applyPlatformEvent(state, PlatformEvent{MouseButtonEvent{.button = MouseButton::Right, .pressed = true}});
    handler.handleInput(state);

    REQUIRE(leftClickCount == 1);
    REQUIRE(rightReleaseCount == 0);

    // Frame 2: Unbind Left, release Right
    handler.unbindMouseButton(MouseButton::Left, InputTrigger::Pressed);

    resetFrameState(state);
    applyPlatformEvent(state, PlatformEvent{MouseButtonEvent{.button = MouseButton::Left, .pressed = true}});
    applyPlatformEvent(state, PlatformEvent{MouseButtonEvent{.button = MouseButton::Right, .pressed = false}});
    handler.handleInput(state);

    REQUIRE(leftClickCount == 1);
    REQUIRE(rightReleaseCount == 1);

    // Frame 3: Clear all bindings
    handler.clear();

    resetFrameState(state);
    applyPlatformEvent(state, PlatformEvent{MouseButtonEvent{.button = MouseButton::Right, .pressed = false}});
    handler.handleInput(state);

    REQUIRE(rightReleaseCount == 1);
}
