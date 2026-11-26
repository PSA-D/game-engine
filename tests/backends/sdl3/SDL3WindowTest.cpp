#include "backends/sdl3/SDL3Window.h"

#include "platform/WindowConfig.h"

#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <string>

using namespace engine;
using namespace engine::backends;
using namespace engine::platform;

TEST_CASE("SDL3Window initializes from WindowConfig", "[window][sdl3]")
{
    WindowConfig config;
    config.title = "Configured SDL3 window";
    config.size = {320, 240};
    config.fullscreen = false;
    config.resizable = true;

    SDL3Window window(config);

    CHECK(window.getTitle() == config.title);
    CHECK(window.getSize() == config.size);
    CHECK(window.getPixelSize().width > 0);
    CHECK(window.getPixelSize().height > 0);
    CHECK(window.getNativeHandle() != nullptr);

    const SDL_WindowFlags flags = SDL_GetWindowFlags(static_cast<SDL_Window*>(window.getNativeHandle()));
    CHECK((flags & SDL_WINDOW_FULLSCREEN) == 0);
    CHECK((flags & SDL_WINDOW_RESIZABLE) != 0);
}

TEST_CASE("SDL3Window setters update window state and config", "[window][sdl3]")
{
    WindowConfig config;
    config.title = "SDL3 window before updates";
    config.size = {320, 240};
    config.resizable = false;
    config.vsync = false;

    SDL3Window window(config);

    window.setTitle("SDL3 window after update");
    CHECK(window.getTitle() == "SDL3 window after update");
    CHECK(window.getConfig().title == "SDL3 window after update");

    const Size newSize{480, 360};
    window.setSize(newSize);
    CHECK(window.getSize() == newSize);
    CHECK(window.getConfig().size == newSize);

    window.setFullscreen(true);
    CHECK(window.getConfig().fullscreen);
    CHECK((SDL_GetWindowFlags(static_cast<SDL_Window*>(window.getNativeHandle())) & SDL_WINDOW_FULLSCREEN) != 0);

    window.setFullscreen(false);
    CHECK_FALSE(window.getConfig().fullscreen);
    CHECK((SDL_GetWindowFlags(static_cast<SDL_Window*>(window.getNativeHandle())) & SDL_WINDOW_FULLSCREEN) == 0);
}

TEST_CASE("SDL3Window applies fullscreen and resizable flags from WindowConfig", "[window][sdl3]")
{
    WindowConfig config;
    config.fullscreen = true;
    config.resizable = false;

    SDL3Window window(config);
    const SDL_WindowFlags flags = SDL_GetWindowFlags(static_cast<SDL_Window*>(window.getNativeHandle()));

    CHECK((flags & SDL_WINDOW_FULLSCREEN) != 0);
    CHECK((flags & SDL_WINDOW_RESIZABLE) == 0);
}
