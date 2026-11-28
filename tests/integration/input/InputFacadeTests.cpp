#include "EngineConfig.h"
#include "core/EngineImpl.h"
#include "facades/InputFacade.h"

#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>

using namespace engine;

namespace
{
    struct SdlGuard
    {
        SdlGuard()
        {
            SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
            if (!SDL_Init(SDL_INIT_VIDEO))
                FAIL("SDL_Init failed: " << SDL_GetError());
        }

        ~SdlGuard()
        {
            SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_LOST, true);
            SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_GAINED, true);
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        SdlGuard(const SdlGuard&) = delete;
        SdlGuard& operator=(const SdlGuard&) = delete;
    };

    void pushKeyEvent(SDL_Scancode scancode, bool down)
    {
        SDL_Event event{};
        event.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
        event.key.scancode = scancode;
        event.key.down = down;
        event.key.repeat = false;
        SDL_PushEvent(&event);
    }

    void pushMouseButtonEvent(Uint8 button, bool down, float x = 0.0f, float y = 0.0f)
    {
        SDL_Event event{};
        event.type = down ? SDL_EVENT_MOUSE_BUTTON_DOWN : SDL_EVENT_MOUSE_BUTTON_UP;
        event.button.button = button;
        event.button.down = down;
        event.button.x = x;
        event.button.y = y;
        SDL_PushEvent(&event);
    }

    void pushMouseMotionEvent(float x, float y, float xrel, float yrel)
    {
        SDL_Event event{};
        event.type = SDL_EVENT_MOUSE_MOTION;
        event.motion.x = x;
        event.motion.y = y;
        event.motion.xrel = xrel;
        event.motion.yrel = yrel;
        SDL_PushEvent(&event);
    }

    void pushMouseWheelEvent(float x, float y)
    {
        SDL_Event event{};
        event.type = SDL_EVENT_MOUSE_WHEEL;
        event.wheel.x = x;
        event.wheel.y = y;
        SDL_PushEvent(&event);
    }

    void pushQuitEvent()
    {
        SDL_Event event{};
        event.type = SDL_EVENT_QUIT;
        REQUIRE(SDL_PushEvent(&event));
    }

    // Removes the OS noise that a freshly created window produces, so that only the
    // events pushed by the test reach the InputSubsystem. Call right after creating the engine.
    void silenceWindowNoise()
    {
        // The test never pushes focus events, so the OS can't produce them either.
        SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_LOST, false);
        SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_GAINED, false);

        // Drop whatever the window creation already queued.
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
    }

    void restoreWindowNoise()
    {
        SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_LOST, true);
        SDL_SetEventEnabled(SDL_EVENT_WINDOW_FOCUS_GAINED, true);
    }

    class SpyKeyListener : public IKeyListener
    {
    public:
        int pressedCount = 0;
        int releasedCount = 0;
        KeyCode lastKey = KeyCode::Unknown;

        void onKeyDown(const platform::KeyEvent& event) override
        {
            ++pressedCount;
            lastKey = event.key;
        }

        void onKeyUp(const platform::KeyEvent&) override
        {
            ++releasedCount;
        }
    };
} // namespace

TEST_CASE("InputFacade integration with EngineImpl and SDL3 backend", "[input][facade][integration]")
{
    SdlGuard sdl;

    EngineConfig config{};
    config.platformBackend = PlatformBackend::SDL3;

    EngineImpl engine{config};
    silenceWindowNoise();

    SECTION("Keyboard press propagates to InputFacade during run()")
    {
        pushKeyEvent(SDL_SCANCODE_SPACE, true);
        pushQuitEvent();

        engine.run();

        REQUIRE(InputFacade::getKey(KeyCode::Space));
        REQUIRE(InputFacade::getKeyDown(KeyCode::Space));
        REQUIRE_FALSE(InputFacade::getKeyUp(KeyCode::Space));
    }

    SECTION("Mouse button press propagates to InputFacade during run()")
    {
        pushMouseButtonEvent(SDL_BUTTON_LEFT, true);
        pushQuitEvent();

        engine.run();

        REQUIRE(InputFacade::getMouseButton(MouseButton::Left));
        REQUIRE(InputFacade::getMouseButtonDown(MouseButton::Left));
        REQUIRE_FALSE(InputFacade::getMouseButtonUp(MouseButton::Left));
    }

    SECTION("Mouse motion and wheel events propagate to InputFacade during run()")
    {
        pushMouseMotionEvent(320.0f, 180.0f, 15.0f, -10.0f);
        pushMouseWheelEvent(0.0f, 2.0f);
        pushQuitEvent();

        engine.run();

        const Vector2 pos = InputFacade::getMousePosition();
        REQUIRE(pos.x == 320.0f);
        REQUIRE(pos.y == 180.0f);

        const Vector2 delta = InputFacade::getMouseDelta();
        REQUIRE(delta.x == 15.0f);
        REQUIRE(delta.y == -10.0f);

        const Vector2 wheel = InputFacade::getMouseWheel();
        REQUIRE(wheel.x == 0.0f);
        REQUIRE(wheel.y == 2.0f);
    }

    SECTION("Key listener receives key down and key up events through full SDL3 pipeline")
    {
        SpyKeyListener listener;
        InputFacade::addKeyListener(&listener);

        pushKeyEvent(SDL_SCANCODE_W, true);
        pushKeyEvent(SDL_SCANCODE_W, false);
        pushQuitEvent();

        engine.run();

        REQUIRE(listener.pressedCount == 1);
        REQUIRE(listener.releasedCount == 1);
        REQUIRE(listener.lastKey == KeyCode::W);

        InputFacade::removeKeyListener(&listener);
    }

    restoreWindowNoise();
}
