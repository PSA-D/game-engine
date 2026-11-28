#include "input/InputSubsystem.h"
#include "platform/IInputSource.h"
#include "platform/INativeEventListener.h"
#include "utils/KeyCode.h"
#include "utils/MouseButton.h"

#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <queue>
#include <vector>

using namespace engine;
using namespace engine::subsystems;

namespace
{
    class FakeInputSource : public platform::IInputSource
    {
    public:
        std::optional<platform::PlatformEvent> pollEvent() override
        {
            if (events_.empty())
                return std::nullopt;

            auto event = events_.front();
            events_.pop();
            return event;
        }

        void addNativeEventListener(platform::INativeEventListener* listener) override
        {
            listeners_.push_back(listener);
        }

        void removeNativeEventListener(platform::INativeEventListener* listener) override
        {
            std::erase(listeners_, listener);
        }

        void pushEvent(const platform::PlatformEvent& event)
        {
            events_.push(event);
        }

    private:
        std::queue<platform::PlatformEvent> events_;
        std::vector<platform::INativeEventListener*> listeners_;
    };

    class SpyKeyListener : public IKeyListener
    {
    public:
        int downCount = 0;
        int upCount = 0;
        KeyCode lastKey = KeyCode::Unknown;

        void onKeyDown(const platform::KeyEvent& event) override
        {
            ++downCount;
            lastKey = event.key;
        }

        void onKeyUp(const platform::KeyEvent& event) override
        {
            ++upCount;
            lastKey = event.key;
        }
    };

    class SpyMouseListener : public IMouseListener
    {
    public:
        int buttonDownCount = 0;
        int buttonUpCount = 0;
        int moveCount = 0;
        int wheelCount = 0;

        void onMouseButtonDown(const platform::MouseButtonEvent&) override
        {
            ++buttonDownCount;
        }
        void onMouseButtonUp(const platform::MouseButtonEvent&) override
        {
            ++buttonUpCount;
        }
        void onMouseMove(const platform::MouseMoveEvent&) override
        {
            ++moveCount;
        }
        void onMouseWheel(const platform::MouseWheelEvent&) override
        {
            ++wheelCount;
        }
    };

    class SpyCommand : public ICommand
    {
    public:
        explicit SpyCommand(int& counter)
            : counter_(counter)
        {
        }
        void execute() override
        {
            ++counter_;
        }

    private:
        int& counter_;
    };
} // namespace

TEST_CASE("InputSubsystem keyboard lifecycle and repeat handling", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    SECTION("Press, hold across frames, and release transitions cleanly")
    {
        // Frame 1: Press Space
        source.pushEvent(platform::KeyEvent{KeyCode::Space, {}, true, false});
        subsystem.update();

        REQUIRE(subsystem.getKey(KeyCode::Space));
        REQUIRE(subsystem.getKeyDown(KeyCode::Space));
        REQUIRE_FALSE(subsystem.getKeyUp(KeyCode::Space));

        // Frame 2: No new events -> getKeyDown resets, getKey remains true
        subsystem.update();

        REQUIRE(subsystem.getKey(KeyCode::Space));
        REQUIRE_FALSE(subsystem.getKeyDown(KeyCode::Space));
        REQUIRE_FALSE(subsystem.getKeyUp(KeyCode::Space));

        // Frame 3: Release Space -> getKeyUp is true for one frame
        source.pushEvent(platform::KeyEvent{KeyCode::Space, {}, false, false});
        subsystem.update();

        REQUIRE_FALSE(subsystem.getKey(KeyCode::Space));
        REQUIRE_FALSE(subsystem.getKeyDown(KeyCode::Space));
        REQUIRE(subsystem.getKeyUp(KeyCode::Space));

        // Frame 4: getKeyUp resets
        subsystem.update();
        REQUIRE_FALSE(subsystem.getKeyUp(KeyCode::Space));
    }

    SECTION("OS key repeat events do not re-trigger getKeyDown")
    {
        // Initial press
        source.pushEvent(platform::KeyEvent{KeyCode::A, {}, true, false});
        subsystem.update();
        REQUIRE(subsystem.getKeyDown(KeyCode::A));

        // Next frame: OS sends repeat event while key is already held
        source.pushEvent(platform::KeyEvent{KeyCode::A, {}, true, true});
        subsystem.update();

        REQUIRE(subsystem.getKey(KeyCode::A));
        REQUIRE_FALSE(subsystem.getKeyDown(KeyCode::A));
    }

    SECTION("Releasing an unpressed key does not trigger getKeyUp")
    {
        source.pushEvent(platform::KeyEvent{KeyCode::W, {}, false, false});
        subsystem.update();

        REQUIRE_FALSE(subsystem.getKey(KeyCode::W));
        REQUIRE_FALSE(subsystem.getKeyUp(KeyCode::W));
    }
}

TEST_CASE("InputSubsystem mouse button, motion, and wheel state", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    SECTION("Mouse button press and release lifecycle")
    {
        source.pushEvent(platform::MouseButtonEvent{MouseButton::Left, true, 10.0f, 20.0f});
        subsystem.update();

        REQUIRE(subsystem.getMouseButton(MouseButton::Left));
        REQUIRE(subsystem.getMouseButtonDown(MouseButton::Left));
        REQUIRE_FALSE(subsystem.getMouseButtonUp(MouseButton::Left));

        // Next frame without events resets getMouseButtonDown
        subsystem.update();
        REQUIRE(subsystem.getMouseButton(MouseButton::Left));
        REQUIRE_FALSE(subsystem.getMouseButtonDown(MouseButton::Left));

        // Release
        source.pushEvent(platform::MouseButtonEvent{MouseButton::Left, false, 10.0f, 20.0f});
        subsystem.update();

        REQUIRE_FALSE(subsystem.getMouseButton(MouseButton::Left));
        REQUIRE(subsystem.getMouseButtonUp(MouseButton::Left));
    }

    SECTION("Mouse delta and wheel accumulate within a frame and reset on next frame")
    {
        source.pushEvent(platform::MouseMoveEvent{{100.0f, 200.0f}, {5.0f, -3.0f}});
        source.pushEvent(platform::MouseMoveEvent{{110.0f, 195.0f}, {10.0f, -5.0f}});
        source.pushEvent(platform::MouseWheelEvent{{0.0f, 1.0f}});
        source.pushEvent(platform::MouseWheelEvent{{0.5f, 2.0f}});
        subsystem.update();

        REQUIRE(subsystem.getMousePosition().x == 110.0f);
        REQUIRE(subsystem.getMousePosition().y == 195.0f);
        REQUIRE(subsystem.getMouseDelta().x == 15.0f);
        REQUIRE(subsystem.getMouseDelta().y == -8.0f);
        REQUIRE(subsystem.getMouseWheel().x == 0.5f);
        REQUIRE(subsystem.getMouseWheel().y == 3.0f);

        // Next frame: position stays, delta and wheel reset to zero
        subsystem.update();
        REQUIRE(subsystem.getMousePosition().x == 110.0f);
        REQUIRE(subsystem.getMousePosition().y == 195.0f);
        REQUIRE(subsystem.getMouseDelta().x == 0.0f);
        REQUIRE(subsystem.getMouseDelta().y == 0.0f);
        REQUIRE(subsystem.getMouseWheel().x == 0.0f);
        REQUIRE(subsystem.getMouseWheel().y == 0.0f);
    }
}

TEST_CASE("InputSubsystem window events, FocusLost, and QuitEvent", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    SECTION("FocusLost releases all held keys and mouse buttons")
    {
        source.pushEvent(platform::KeyEvent{KeyCode::W, {}, true, false});
        source.pushEvent(platform::MouseButtonEvent{MouseButton::Right, true, 0.0f, 0.0f});
        subsystem.update();

        REQUIRE(subsystem.getKey(KeyCode::W));
        REQUIRE(subsystem.getMouseButton(MouseButton::Right));

        // Window loses focus -> held inputs must be released
        source.pushEvent(platform::WindowEvent{platform::WindowEventKind::FocusLost, {800, 600}});
        subsystem.update();

        REQUIRE_FALSE(subsystem.getKey(KeyCode::W));
        REQUIRE(subsystem.getKeyUp(KeyCode::W));
        REQUIRE_FALSE(subsystem.getMouseButton(MouseButton::Right));
        REQUIRE(subsystem.getMouseButtonUp(MouseButton::Right));
    }

    SECTION("takeWindowEvents returns queued window events and clears the queue")
    {
        source.pushEvent(platform::WindowEvent{platform::WindowEventKind::Resized, {1280, 720}});
        source.pushEvent(platform::WindowEvent{platform::WindowEventKind::Minimized, {0, 0}});
        subsystem.update();

        const auto events = subsystem.takeWindowEvents();
        REQUIRE(events.size() == 2);
        REQUIRE(events[0].kind == platform::WindowEventKind::Resized);
        REQUIRE(events[0].size.width == 1280);
        REQUIRE(events[1].kind == platform::WindowEventKind::Minimized);

        // Second call must return an empty vector
        REQUIRE(subsystem.takeWindowEvents().empty());
    }

    SECTION("QuitEvent sets isQuitRequested to true")
    {
        REQUIRE_FALSE(subsystem.isQuitRequested());

        source.pushEvent(platform::QuitEvent{});
        subsystem.update();

        REQUIRE(subsystem.isQuitRequested());
    }
}

TEST_CASE("InputSubsystem dispatches events to registered listeners", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    SpyKeyListener keyListener;
    SpyMouseListener mouseListener;

    subsystem.addKeyListener(&keyListener);
    subsystem.addMouseListener(&mouseListener);

    source.pushEvent(platform::KeyEvent{KeyCode::D, {}, true, false});
    source.pushEvent(platform::KeyEvent{KeyCode::D, {}, false, false});
    source.pushEvent(platform::MouseButtonEvent{MouseButton::Middle, true, 0.0f, 0.0f});
    source.pushEvent(platform::MouseButtonEvent{MouseButton::Middle, false, 0.0f, 0.0f});
    source.pushEvent(platform::MouseMoveEvent{{50.0f, 50.0f}, {1.0f, 1.0f}});
    source.pushEvent(platform::MouseWheelEvent{{0.0f, -1.0f}});
    subsystem.update();

    REQUIRE(keyListener.downCount == 1);
    REQUIRE(keyListener.upCount == 1);
    REQUIRE(keyListener.lastKey == KeyCode::D);

    REQUIRE(mouseListener.buttonDownCount == 1);
    REQUIRE(mouseListener.buttonUpCount == 1);
    REQUIRE(mouseListener.moveCount == 1);
    REQUIRE(mouseListener.wheelCount == 1);

    // Unregistering stops further callbacks
    subsystem.removeKeyListener(&keyListener);
    subsystem.removeMouseListener(&mouseListener);

    source.pushEvent(platform::KeyEvent{KeyCode::D, {}, true, false});
    source.pushEvent(platform::MouseMoveEvent{{60.0f, 60.0f}, {10.0f, 10.0f}});
    subsystem.update();

    REQUIRE(keyListener.downCount == 1);
    REQUIRE(mouseListener.moveCount == 1);
}

TEST_CASE("InputSubsystem executes and manages command bindings", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    int jumpCount = 0;
    int shootCount = 0;

    subsystem.bindKey(KeyCode::Space, InputTrigger::Pressed, std::make_unique<SpyCommand>(jumpCount));
    subsystem.bindMouseButton(MouseButton::Left, InputTrigger::Pressed, std::make_unique<SpyCommand>(shootCount));

    source.pushEvent(platform::KeyEvent{KeyCode::Space, {}, true, false});
    source.pushEvent(platform::MouseButtonEvent{MouseButton::Left, true, 0.0f, 0.0f});
    subsystem.update();

    REQUIRE(jumpCount == 1);
    REQUIRE(shootCount == 1);

    // Unbind key and clear all remaining bindings
    subsystem.unbindKey(KeyCode::Space, InputTrigger::Pressed);
    subsystem.clear();

    // Release and press again -> neither command should fire
    source.pushEvent(platform::KeyEvent{KeyCode::Space, {}, false, false});
    source.pushEvent(platform::MouseButtonEvent{MouseButton::Left, false, 0.0f, 0.0f});
    subsystem.update();

    source.pushEvent(platform::KeyEvent{KeyCode::Space, {}, true, false});
    source.pushEvent(platform::MouseButtonEvent{MouseButton::Left, true, 0.0f, 0.0f});
    subsystem.update();

    REQUIRE(jumpCount == 1);
    REQUIRE(shootCount == 1);
}

TEST_CASE("InputSubsystem guards against out-of-bounds KeyCode and MouseButton values", "[input][subsystem]")
{
    FakeInputSource source;
    InputSubsystem subsystem{source};

    const auto invalidKey = KeyCode::Count;
    const auto invalidButton = MouseButton::Count;

    source.pushEvent(platform::KeyEvent{invalidKey, {}, true, false});
    source.pushEvent(platform::MouseButtonEvent{invalidButton, true, 0.0f, 0.0f});
    subsystem.update();

    REQUIRE_FALSE(subsystem.getKey(invalidKey));
    REQUIRE_FALSE(subsystem.getKeyDown(invalidKey));
    REQUIRE_FALSE(subsystem.getKeyUp(invalidKey));
    REQUIRE_FALSE(subsystem.getMouseButton(invalidButton));
    REQUIRE_FALSE(subsystem.getMouseButtonDown(invalidButton));
    REQUIRE_FALSE(subsystem.getMouseButtonUp(invalidButton));
}
