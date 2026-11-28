#include "backends/sdl3/SDL3InputSource.h"
#include "backends/sdl3/SDL3Window.h"
#include "input/InputHandler.h"
#include "input/InputSubsystem.h"
#include "input/commands/FunctionCommand.h"
#include "platform/IWindow.h"

#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace engine::backends;
using namespace engine::subsystems;
using namespace engine::platform;
using namespace engine;

namespace
{
    struct SDLTestFixture
    {
        SDLTestFixture()
        {
            // Events subsystem is enough for SDL_PushEvent / SDL_PollEvent (works headless on Linux/Windows CI)
            REQUIRE(SDL_InitSubSystem(SDL_INIT_EVENTS));
            SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
        }

        ~SDLTestFixture()
        {
            SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
            SDL_QuitSubSystem(SDL_INIT_EVENTS);
        }
    };

    void pushSDLKeyEvent(SDL_Scancode scancode, bool down, bool repeat = false)
    {
        SDL_Event event{};
        event.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
        event.key.scancode = scancode;
        event.key.down = down;
        event.key.repeat = repeat;
        event.key.mod = SDL_KMOD_NONE;
        REQUIRE(SDL_PushEvent(&event));
    }

    void pushSDLMouseButtonEvent(Uint8 button, bool down, float x = 100.0f, float y = 200.0f)
    {
        SDL_Event event{};
        event.type = down ? SDL_EVENT_MOUSE_BUTTON_DOWN : SDL_EVENT_MOUSE_BUTTON_UP;
        event.button.button = button;
        event.button.down = down;
        event.button.x = x;
        event.button.y = y;
        REQUIRE(SDL_PushEvent(&event));
    }

    void pushSDLMouseMotionEvent(float x, float y, float xrel, float yrel)
    {
        SDL_Event event{};
        event.type = SDL_EVENT_MOUSE_MOTION;
        event.motion.x = x;
        event.motion.y = y;
        event.motion.xrel = xrel;
        event.motion.yrel = yrel;
        REQUIRE(SDL_PushEvent(&event));
    }

    void pushSDLWindowEvent(SDL_EventType type)
    {
        SDL_Event event{};
        event.type = type;
        REQUIRE(SDL_PushEvent(&event));
    }
} // namespace

TEST_CASE("SDL3InputSource integrates end-to-end with InputSubsystem and InputHandler", "[input][sdl3][integration]")
{
    SDLTestFixture sdl;
    WindowConfig windowConfig{1280, 720};
    std::unique_ptr<SDL3Window> window = std::make_unique<SDL3Window>(windowConfig);
    SDL3InputSource inputSource(window.get());
    InputSubsystem inputSubsystem(inputSource);

    int jumpPressedCount = 0;
    int jumpHeldCount = 0;
    int jumpReleasedCount = 0;
    int fireCount = 0;

    auto& handler = inputSubsystem.getHandler();
    handler.bindKey(
        KeyCode::Space, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&]() { ++jumpPressedCount; }));
    handler.bindKey(KeyCode::Space, InputTrigger::Held, std::make_unique<FunctionCommand>([&]() { ++jumpHeldCount; }));
    handler.bindKey(
        KeyCode::Space, InputTrigger::Released, std::make_unique<FunctionCommand>([&]() { ++jumpReleasedCount; }));
    handler.bindMouseButton(
        MouseButton::Left, InputTrigger::Pressed, std::make_unique<FunctionCommand>([&]() { ++fireCount; }));

    // Frame 1: Push real SDL key down + mouse button down + mouse motion
    pushSDLKeyEvent(SDL_SCANCODE_SPACE, true, false);
    pushSDLMouseButtonEvent(SDL_BUTTON_LEFT, true, 320.0f, 240.0f);
    pushSDLMouseMotionEvent(320.0f, 240.0f, 15.0f, -5.0f);

    inputSubsystem.update();

    REQUIRE(inputSubsystem.getKey(KeyCode::Space));
    REQUIRE(inputSubsystem.getKeyDown(KeyCode::Space));
    REQUIRE_FALSE(inputSubsystem.getKeyUp(KeyCode::Space));
    REQUIRE(inputSubsystem.getMouseButton(MouseButton::Left));
    REQUIRE(inputSubsystem.getMouseButtonDown(MouseButton::Left));
    REQUIRE(inputSubsystem.getMousePosition().x == 320.0f);
    REQUIRE(inputSubsystem.getMousePosition().y == 240.0f);
    REQUIRE(inputSubsystem.getMouseDelta().x == 15.0f);
    REQUIRE(inputSubsystem.getMouseDelta().y == -5.0f);

    REQUIRE(jumpPressedCount == 1);
    REQUIRE(jumpHeldCount == 1);
    REQUIRE(jumpReleasedCount == 0);
    REQUIRE(fireCount == 1);

    // Frame 2: SDL OS key repeat arrives -> getKeyDown and Pressed command must NOT re-trigger
    pushSDLKeyEvent(SDL_SCANCODE_SPACE, true, true);
    inputSubsystem.update();

    REQUIRE(inputSubsystem.getKey(KeyCode::Space));
    REQUIRE_FALSE(inputSubsystem.getKeyDown(KeyCode::Space));
    REQUIRE(inputSubsystem.getMouseDelta().x == 0.0f);
    REQUIRE(inputSubsystem.getMouseDelta().y == 0.0f);

    REQUIRE(jumpPressedCount == 1);
    REQUIRE(jumpHeldCount == 2);
    REQUIRE(jumpReleasedCount == 0);

    // Frame 3: Push SDL key up -> getKeyUp is true for 1 frame and Released command fires
    pushSDLKeyEvent(SDL_SCANCODE_SPACE, false, false);
    inputSubsystem.update();

    REQUIRE_FALSE(inputSubsystem.getKey(KeyCode::Space));
    REQUIRE_FALSE(inputSubsystem.getKeyDown(KeyCode::Space));
    REQUIRE(inputSubsystem.getKeyUp(KeyCode::Space));

    REQUIRE(jumpPressedCount == 1);
    REQUIRE(jumpHeldCount == 2);
    REQUIRE(jumpReleasedCount == 1);

    // Frame 4: No events -> getKeyUp resets
    inputSubsystem.update();

    REQUIRE_FALSE(inputSubsystem.getKeyUp(KeyCode::Space));
}

TEST_CASE("SDL3InputSource native event listener consumes events before InputSubsystem sees them",
          "[input][sdl3][integration]")
{
    class ConsumingNativeListener : public INativeEventListener
    {
    public:
        bool consume = true;
        int seenCount = 0;

        bool onNativeEvent(const void*) override
        {
            ++seenCount;
            return consume;
        }
    };

    SDLTestFixture sdl;
    WindowConfig windowConfig{1280, 720};
    std::unique_ptr<SDL3Window> window = std::make_unique<SDL3Window>(windowConfig);
    SDL3InputSource inputSource(window.get());
    InputSubsystem inputSubsystem(inputSource);

    ConsumingNativeListener uiListener;
    inputSource.addNativeEventListener(&uiListener);

    pushSDLKeyEvent(SDL_SCANCODE_SPACE, true, false);
    inputSubsystem.update();

    REQUIRE(uiListener.seenCount == 1);
    REQUIRE_FALSE(inputSubsystem.getKey(KeyCode::Space));
    REQUIRE_FALSE(inputSubsystem.getKeyDown(KeyCode::Space));

    inputSource.removeNativeEventListener(&uiListener);
}

TEST_CASE("SDL3InputSource translates FocusLost and Quit events through InputSubsystem", "[input][sdl3][integration]")
{
    SDLTestFixture sdl;
    WindowConfig windowConfig{1280, 720};
    std::unique_ptr<SDL3Window> window = std::make_unique<SDL3Window>(windowConfig);
    SDL3InputSource inputSource(window.get());
    InputSubsystem inputSubsystem(inputSource);

    pushSDLKeyEvent(SDL_SCANCODE_W, true, false);
    inputSubsystem.update();
    REQUIRE(inputSubsystem.getKey(KeyCode::W));

    pushSDLWindowEvent(SDL_EVENT_WINDOW_FOCUS_LOST);
    pushSDLWindowEvent(SDL_EVENT_QUIT);
    inputSubsystem.update();

    REQUIRE_FALSE(inputSubsystem.getKey(KeyCode::W));
    REQUIRE(inputSubsystem.getKeyUp(KeyCode::W));
    REQUIRE(inputSubsystem.isQuitRequested());

    const auto windowEvents = inputSubsystem.takeWindowEvents();
    REQUIRE(windowEvents.size() == 1);
    REQUIRE(windowEvents.front().kind == WindowEventKind::FocusLost);
}
