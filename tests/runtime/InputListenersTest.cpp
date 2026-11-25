#include "../../engine/include/IKeyListener.h"
#include "IMouseListener.h"
#include "input/InputSubsystem.h"
#include "input/ListenersList.h"
#include "platform/IInputSource.h"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <vector>

namespace
{
    // Mock IInputSource so we can feed controlled events in unit tests without SDL3
    class MockInputSource : public engine::platform::IInputSource
    {
    public:
        void pushEvent(engine::platform::PlatformEvent event)
        {
            events_.push(event);
        }

        std::optional<engine::platform::PlatformEvent> pollEvent() override
        {
            if (events_.empty())
                return std::nullopt;

            auto ev = events_.front();
            events_.pop();
            return ev;
        }

        // Add stubs here if IInputSource has other pure virtual methods
        // (e.g., addNativeEventListener / removeNativeEventListener)
        void addNativeEventListener(engine::platform::INativeEventListener* /*listener*/) override {}
        void removeNativeEventListener(engine::platform::INativeEventListener* /*listener*/) override {}

    private:
        std::queue<engine::platform::PlatformEvent> events_;
    };

    // Spy implementation of IKeyListener
    class SpyKeyListener : public engine::IKeyListener
    {
    public:
        int keyDownCount = 0;
        int keyUpCount = 0;
        engine::platform::KeyEvent lastEvent{};
        std::function<void()> onCallback;

        void onKeyDown(const engine::platform::KeyEvent& event) override
        {
            ++keyDownCount;
            lastEvent = event;
            if (onCallback)
                onCallback();
        }

        void onKeyUp(const engine::platform::KeyEvent& event) override
        {
            ++keyUpCount;
            lastEvent = event;
        }
    };

    // Spy implementation of IMouseListener
    class SpyMouseListener : public engine::IMouseListener
    {
    public:
        int buttonDownCount = 0;
        int buttonUpCount = 0;
        int moveCount = 0;
        int wheelCount = 0;
        std::function<void()> onCallback;

        void onMouseButtonDown(const engine::platform::MouseButtonEvent&) override
        {
            ++buttonDownCount;
            if (onCallback)
                onCallback();
        }

        void onMouseButtonUp(const engine::platform::MouseButtonEvent&) override
        {
            ++buttonUpCount;
        }

        void onMouseMove(const engine::platform::MouseMoveEvent&) override
        {
            ++moveCount;
            if (onCallback)
                onCallback();
        }

        void onMouseWheel(const engine::platform::MouseWheelEvent&) override
        {
            ++wheelCount;
            if (onCallback)
                onCallback();
        }
    };
} // namespace

TEST_CASE("ListenersList: registered listener receives events, unregistered does not", "[input][listeners]")
{
    ListenersList<engine::IKeyListener> list;
    SpyKeyListener listenerA;
    SpyKeyListener listenerB;

    engine::platform::KeyEvent keyEvent{};

    SECTION("Registered listener receives dispatched event")
    {
        list.add(&listenerA);
        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); });

        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0);
    }

    SECTION("Duplicate registration only delivers event once")
    {
        list.add(&listenerA);
        list.add(&listenerA);
        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); });

        REQUIRE(listenerA.keyDownCount == 1);
    }

    SECTION("Unregistered listener no longer receives events")
    {
        list.add(&listenerA);
        list.add(&listenerB);
        list.remove(&listenerA);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); });

        REQUIRE(listenerA.keyDownCount == 0);
        REQUIRE(listenerB.keyDownCount == 1);
    }
}

TEST_CASE("ListenersList: unsubscribing and subscribing during dispatch does not crash", "[input][listeners]")
{
    ListenersList<engine::IKeyListener> list;
    SpyKeyListener listenerA;
    SpyKeyListener listenerB;
    SpyKeyListener listenerC;

    engine::platform::KeyEvent keyEvent{};

    SECTION("Listener unsubscribes itself during dispatch")
    {
        list.add(&listenerA);
        list.add(&listenerB);

        listenerA.onCallback = [&]() { list.remove(&listenerA); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); }));

        // Both A and B were active when dispatch started, and A is now removed for future dispatches
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 1);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); });
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 2);
    }

    SECTION("Listener unsubscribes another listener that has not fired yet")
    {
        list.add(&listenerA);
        list.add(&listenerB);
        list.add(&listenerC);

        listenerA.onCallback = [&]() { list.remove(&listenerB); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); }));

        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0); // Skipped immediately because it was queued for removal
        REQUIRE(listenerC.keyDownCount == 1);
    }

    SECTION("Listener subscribes a new listener during dispatch")
    {
        list.add(&listenerA);

        listenerA.onCallback = [&]() { list.add(&listenerB); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); }));

        // Newly added listener does not fire during the current dispatch, but fires on the next one
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(keyEvent); });
        REQUIRE(listenerA.keyDownCount == 2);
        REQUIRE(listenerB.keyDownCount == 1);
    }
}

TEST_CASE("InputSubsystem: dispatches platform events to registered listeners safely", "[input][subsystem]")
{
    MockInputSource source;
    engine::subsystems::InputSubsystem inputSubsystem{source};

    SpyKeyListener keyListener1;
    SpyKeyListener keyListener2;
    SpyMouseListener mouseListener;

    inputSubsystem.addKeyListener(&keyListener1);
    inputSubsystem.addKeyListener(&keyListener2);
    inputSubsystem.addMouseListener(&mouseListener);

    SECTION("Registered listeners receive polled events and unregistered do not")
    {
        inputSubsystem.removeKeyListener(&keyListener2);

        source.pushEvent(
            engine::platform::KeyEvent{.key = engine::KeyCode::Unknown, .mods = {}, .pressed = true, .repeat = false});
        inputSubsystem.update();

        REQUIRE(keyListener1.keyDownCount == 1);
        REQUIRE(keyListener2.keyDownCount == 0);

        source.pushEvent(engine::platform::MouseMoveEvent{});
        inputSubsystem.update();

        REQUIRE(mouseListener.moveCount == 1);
    }

    SECTION("Unsubscribing from InputSubsystem mid-event does not crash")
    {
        keyListener1.onCallback = [&]
        {
            inputSubsystem.removeKeyListener(&keyListener1);
            inputSubsystem.removeKeyListener(&keyListener2);
        };

        source.pushEvent(
            engine::platform::KeyEvent{.key = engine::KeyCode::Unknown, .mods = {}, .pressed = true, .repeat = false});
        REQUIRE_NOTHROW(inputSubsystem.update());

        REQUIRE(keyListener1.keyDownCount == 1);
        REQUIRE(keyListener2.keyDownCount == 0);

        // Next event should not reach either removed listener
        source.pushEvent(engine::platform::KeyEvent{});
        inputSubsystem.update();

        REQUIRE(keyListener1.keyDownCount == 1);
        REQUIRE(keyListener2.keyDownCount == 0);
    }
}
