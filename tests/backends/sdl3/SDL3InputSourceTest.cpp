#include "backends/sdl3/SDL3InputSource.h"

#include "platform/Events.h"
#include "platform/INativeEventListener.h"
#include "platform/IWindow.h"
#include "utils/Vector2.h"

#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <variant>

using namespace engine::backends;
using namespace engine;
using namespace engine::platform;
using Catch::Matchers::WithinRel;

namespace
{
    class MockWindow final : public IWindow
    {
    public:
        std::string getTitle() const override
        {
            return "Mock";
        }

        Size getSize() const override
        {
            return {640, 480};
        }

        Size getPixelSize() const override
        {
            return {640, 480};
        }

        void setTitle(const std::string&) override {}

        void setFullscreen(bool) override {}

        void setSize(Size) override {}
    };

    class MockNativeEventListener : public INativeEventListener
    {
    public:
        int callCount = 0;
        Uint32 lastEventType = 0;
        bool shouldConsume = false;

        bool onNativeEvent(const void* nativeEvent) override
        {
            ++callCount;
            if (nativeEvent != nullptr)
            {
                lastEventType = static_cast<const SDL_Event*>(nativeEvent)->type;
            }
            return shouldConsume;
        }
    };

    MockWindow& getDummyWindow()
    {
        static MockWindow dummy{};
        return dummy;
    }
} // namespace

TEST_CASE("SDL3InputSource polls and translates SDL events", "[input][sdl3]")
{
    REQUIRE(SDL_InitSubSystem(SDL_INIT_EVENTS));

    SDL_Event discard;
    while (SDL_PollEvent(&discard))
    {
    }

    SDL3InputSource inputSource(getDummyWindow());

    SECTION("Returns std::nullopt when event queue is empty")
    {
        auto event = inputSource.pollEvent();
        REQUIRE_FALSE(event.has_value());
    }

    SECTION("Translates SDL_EVENT_QUIT into QuitEvent")
    {
        SDL_Event sdlEvent{};
        sdlEvent.type = SDL_EVENT_QUIT;
        REQUIRE(SDL_PushEvent(&sdlEvent));

        auto event = inputSource.pollEvent();
        REQUIRE(event.has_value());
        REQUIRE(std::holds_alternative<QuitEvent>(*event));
    }

    SECTION("Translates keyboard events with modifiers")
    {
        SDL_Event sdlEvent{};
        sdlEvent.type = SDL_EVENT_KEY_DOWN;
        sdlEvent.key.scancode = SDL_SCANCODE_W;
        sdlEvent.key.down = true;
        sdlEvent.key.repeat = true;
        sdlEvent.key.mod = SDL_KMOD_LSHIFT | SDL_KMOD_LCTRL;
        REQUIRE(SDL_PushEvent(&sdlEvent));

        auto event = inputSource.pollEvent();
        REQUIRE(event.has_value());
        REQUIRE(std::holds_alternative<KeyEvent>(*event));

        const auto& keyEvent = std::get<KeyEvent>(*event);
        REQUIRE(keyEvent.key == KeyCode::W);
        REQUIRE(keyEvent.pressed);
        REQUIRE(keyEvent.repeat);
        REQUIRE(keyEvent.mods.shift);
        REQUIRE(keyEvent.mods.ctrl);
        REQUIRE_FALSE(keyEvent.mods.alt);
        REQUIRE_FALSE(keyEvent.mods.gui);
    }

    SECTION("Translates mouse button events")
    {
        SDL_Event sdlEvent{};
        sdlEvent.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
        sdlEvent.button.button = SDL_BUTTON_RIGHT;
        sdlEvent.button.down = true;
        sdlEvent.button.x = 128.5f;
        sdlEvent.button.y = 256.0f;
        REQUIRE(SDL_PushEvent(&sdlEvent));

        auto event = inputSource.pollEvent();
        REQUIRE(event.has_value());
        REQUIRE(std::holds_alternative<MouseButtonEvent>(*event));

        const auto& btnEvent = std::get<MouseButtonEvent>(*event);
        REQUIRE(btnEvent.button == MouseButton::Right);
        REQUIRE(btnEvent.pressed);
        REQUIRE_THAT(btnEvent.position.x, WithinRel(128.5f));
        REQUIRE_THAT(btnEvent.position.y, WithinRel(256.0f));
    }

    SECTION("Translates mouse motion and mouse wheel events")
    {
        SDL_Event moveEvent{};
        moveEvent.type = SDL_EVENT_MOUSE_MOTION;
        moveEvent.motion.x = 100.0f;
        moveEvent.motion.y = 200.0f;
        moveEvent.motion.xrel = 5.0f;
        moveEvent.motion.yrel = -3.0f;
        REQUIRE(SDL_PushEvent(&moveEvent));

        SDL_Event wheelEvent{};
        wheelEvent.type = SDL_EVENT_MOUSE_WHEEL;
        wheelEvent.wheel.x = 1.0f;
        wheelEvent.wheel.y = -2.0f;
        REQUIRE(SDL_PushEvent(&wheelEvent));

        auto first = inputSource.pollEvent();
        REQUIRE(first.has_value());
        REQUIRE(std::holds_alternative<MouseMoveEvent>(*first));
        const auto& m = std::get<MouseMoveEvent>(*first);
        REQUIRE_THAT(m.position.x, WithinRel(100.0f));
        REQUIRE_THAT(m.position.y, WithinRel(200.0f));
        REQUIRE_THAT(m.difference.x, WithinRel(5.0f));
        REQUIRE_THAT(m.difference.y, WithinRel(-3.0f));

        auto second = inputSource.pollEvent();
        REQUIRE(second.has_value());
        REQUIRE(std::holds_alternative<MouseWheelEvent>(*second));
        const auto& w = std::get<MouseWheelEvent>(*second);
        REQUIRE_THAT(w.scroll.x, WithinRel(1.0f));
        REQUIRE_THAT(w.scroll.y, WithinRel(-2.0f));
    }

    SECTION("Translates window resized events")
    {
        SDL_Event sdlEvent{};
        sdlEvent.type = SDL_EVENT_WINDOW_RESIZED;
        sdlEvent.window.data1 = 1920;
        sdlEvent.window.data2 = 1080;
        REQUIRE(SDL_PushEvent(&sdlEvent));

        auto event = inputSource.pollEvent();
        REQUIRE(event.has_value());
        REQUIRE(std::holds_alternative<WindowEvent>(*event));

        const auto& winEvent = std::get<WindowEvent>(*event);
        REQUIRE(winEvent.kind == WindowEventKind::Resized);
        REQUIRE_THAT(winEvent.size.x, WithinRel(1920.0f));
        REQUIRE_THAT(winEvent.size.y, WithinRel(1080.0f));
    }

    SECTION("Skips unhandled SDL events and returns next valid event")
    {
        SDL_Event ignoredEvent{};
        ignoredEvent.type = SDL_EVENT_CLIPBOARD_UPDATE;
        REQUIRE(SDL_PushEvent(&ignoredEvent));

        SDL_Event quitEvent{};
        quitEvent.type = SDL_EVENT_QUIT;
        REQUIRE(SDL_PushEvent(&quitEvent));

        auto event = inputSource.pollEvent();
        REQUIRE(event.has_value());
        REQUIRE(std::holds_alternative<QuitEvent>(*event));
    }

    SECTION("Forwards raw events to INativeEventListeners and removes them cleanly")
    {
        MockNativeEventListener listener;
        inputSource.addNativeEventListener(&listener);

        SDL_Event sdlEvent{};
        sdlEvent.type = SDL_EVENT_QUIT;
        REQUIRE(SDL_PushEvent(&sdlEvent));

        inputSource.pollEvent();
        REQUIRE(listener.callCount == 1);
        REQUIRE(listener.lastEventType == SDL_EVENT_QUIT);

        inputSource.removeNativeEventListener(&listener);
        REQUIRE(SDL_PushEvent(&sdlEvent));

        inputSource.pollEvent();
        REQUIRE(listener.callCount == 1);
    }

    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}
