#include "platform/IInputSource.h"
#include "runtime/IKeyListener.h"
#include "runtime/IMouseListener.h"
#include "subsystems/input/InputSubsystem.h"
#include "subsystems/input/ListenersList.h"

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

    // Maakt een toets-event zoals de input source het zou geven.
    engine::platform::KeyEvent keyEvent(engine::KeyCode key, bool pressed, bool repeat = false)
    {
        engine::platform::KeyEvent event{};
        event.key = key;
        event.pressed = pressed;
        event.repeat = repeat;
        return event;
    }

    // Spy implementation of IKeyListener
    class SpyKeyListener : public engine::IKeyListener
    {
    public:
        int keyDownCount = 0;
        int keyUpCount = 0;
        engine::KeyCode lastKey = engine::KeyCode::Unknown;
        std::function<void()> onCallback;

        void onKeyDown(engine::KeyCode key) override
        {
            ++keyDownCount;
            lastKey = key;
            if (onCallback)
                onCallback();
        }

        void onKeyUp(engine::KeyCode key) override
        {
            ++keyUpCount;
            lastKey = key;
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
        engine::MouseButton lastButton{};
        engine::Vector2 lastPosition{};
        float lastScroll = 0.0F;
        std::function<void()> onCallback;

        void onMouseButtonDown(engine::MouseButton button) override
        {
            ++buttonDownCount;
            lastButton = button;
            if (onCallback)
                onCallback();
        }

        void onMouseButtonUp(engine::MouseButton button) override
        {
            ++buttonUpCount;
            lastButton = button;
        }

        void onMouseMove(const engine::Vector2& position) override
        {
            ++moveCount;
            lastPosition = position;
            if (onCallback)
                onCallback();
        }

        void onMouseWheel(float scrollDelta) override
        {
            ++wheelCount;
            lastScroll = scrollDelta;
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

    const engine::KeyCode key = engine::KeyCode::A;

    SECTION("Registered listener receives dispatched event")
    {
        list.add(&listenerA);
        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); });

        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0);
    }

    SECTION("Duplicate registration only delivers event once")
    {
        list.add(&listenerA);
        list.add(&listenerA);
        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); });

        REQUIRE(listenerA.keyDownCount == 1);
    }

    SECTION("Unregistered listener no longer receives events")
    {
        list.add(&listenerA);
        list.add(&listenerB);
        list.remove(&listenerA);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); });

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

    const engine::KeyCode key = engine::KeyCode::A;

    SECTION("Listener unsubscribes itself during dispatch")
    {
        list.add(&listenerA);
        list.add(&listenerB);

        listenerA.onCallback = [&]() { list.remove(&listenerA); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); }));

        // Both A and B were active when dispatch started, and A is now removed for future dispatches
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 1);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); });
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 2);
    }

    SECTION("Listener unsubscribes another listener that has not fired yet")
    {
        list.add(&listenerA);
        list.add(&listenerB);
        list.add(&listenerC);

        listenerA.onCallback = [&]() { list.remove(&listenerB); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); }));

        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0); // Skipped immediately because it was queued for removal
        REQUIRE(listenerC.keyDownCount == 1);
    }

    SECTION("Listener subscribes a new listener during dispatch")
    {
        list.add(&listenerA);

        listenerA.onCallback = [&]() { list.add(&listenerB); };

        REQUIRE_NOTHROW(list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); }));

        // Newly added listener does not fire during the current dispatch, but fires on the next one
        REQUIRE(listenerA.keyDownCount == 1);
        REQUIRE(listenerB.keyDownCount == 0);

        list.dispatch([&](engine::IKeyListener& l) { l.onKeyDown(key); });
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

        source.pushEvent(keyEvent(engine::KeyCode::A, true));
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

        source.pushEvent(keyEvent(engine::KeyCode::A, true));
        REQUIRE_NOTHROW(inputSubsystem.update());

        REQUIRE(keyListener1.keyDownCount == 1);
        REQUIRE(keyListener2.keyDownCount == 0);

        // Next event should not reach either removed listener
        source.pushEvent(keyEvent(engine::KeyCode::A, true));
        inputSubsystem.update();

        REQUIRE(keyListener1.keyDownCount == 1);
        REQUIRE(keyListener2.keyDownCount == 0);
    }
}

TEST_CASE("InputSubsystem: stuurt het juiste event naar de listeners", "[input][subsystem]")
{
    MockInputSource source;
    engine::subsystems::InputSubsystem inputSubsystem{source};
    SpyKeyListener keyListener;
    SpyMouseListener mouseListener;
    inputSubsystem.addKeyListener(&keyListener);
    inputSubsystem.addMouseListener(&mouseListener);

    SECTION("Indrukken gaat naar onKeyDown, loslaten naar onKeyUp, met de juiste toets")
    {
        source.pushEvent(keyEvent(engine::KeyCode::Space, true));
        inputSubsystem.update();

        REQUIRE(keyListener.keyDownCount == 1);
        REQUIRE(keyListener.keyUpCount == 0);
        REQUIRE(keyListener.lastKey == engine::KeyCode::Space);

        source.pushEvent(keyEvent(engine::KeyCode::Space, false));
        inputSubsystem.update();

        REQUIRE(keyListener.keyDownCount == 1);
        REQUIRE(keyListener.keyUpCount == 1);
    }

    SECTION("Een toets vasthouden (repeat) geeft geen extra onKeyDown")
    {
        source.pushEvent(keyEvent(engine::KeyCode::A, true));
        source.pushEvent(keyEvent(engine::KeyCode::A, true, true));
        inputSubsystem.update();

        REQUIRE(keyListener.keyDownCount == 1);
    }

    SECTION("Alle events in één frame bereiken de listeners en de toestand")
    {
        source.pushEvent(keyEvent(engine::KeyCode::A, true));
        source.pushEvent(keyEvent(engine::KeyCode::Escape, true));
        inputSubsystem.update();

        REQUIRE(keyListener.keyDownCount == 2);
        REQUIRE(inputSubsystem.getKey(engine::KeyCode::A));
        REQUIRE(inputSubsystem.getKey(engine::KeyCode::Escape));
    }

    SECTION("Muisknop, beweging en scrollwiel komen met de juiste waarden aan")
    {
        engine::platform::MouseButtonEvent down{};
        down.button = engine::MouseButton::Right;
        down.pressed = true;
        engine::platform::MouseButtonEvent up = down;
        up.pressed = false;
        engine::platform::MouseMoveEvent move{};
        move.position = {12.0F, 34.0F};
        engine::platform::MouseWheelEvent wheel{};
        wheel.scroll = {0.0F, -1.0F};

        source.pushEvent(down);
        source.pushEvent(up);
        source.pushEvent(move);
        source.pushEvent(wheel);
        inputSubsystem.update();

        REQUIRE(mouseListener.buttonDownCount == 1);
        REQUIRE(mouseListener.buttonUpCount == 1);
        REQUIRE(mouseListener.lastButton == engine::MouseButton::Right);
        REQUIRE(mouseListener.moveCount == 1);
        REQUIRE(mouseListener.lastPosition == engine::Vector2{12.0F, 34.0F});
        REQUIRE(mouseListener.wheelCount == 1);
        REQUIRE(mouseListener.lastScroll == -1.0F);
    }
}
