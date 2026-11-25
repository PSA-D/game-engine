#include "backends/sdl3/SDL3KeyMap.h"

#include "backends/sdl3/SDL3InputSource.h"
#include "utils/KeyCode.h"
#include "utils/MouseButton.h"

#include <SDL3/SDL_scancode.h>
#include <catch2/catch_test_macros.hpp>

using namespace engine::backends;
using engine::KeyCode;

TEST_CASE("SDL3KeyMap translates SDL scancodes to engine KeyCodes", "[input][keymap]")
{
    SECTION("Maps letter keys")
    {
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_A) == KeyCode::A);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_W) == KeyCode::W);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_S) == KeyCode::S);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_D) == KeyCode::D);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_Z) == KeyCode::Z);
    }

    SECTION("Maps number row keys")
    {
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_0) == KeyCode::Num0);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_1) == KeyCode::Num1);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_5) == KeyCode::Num5);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_9) == KeyCode::Num9);
    }

    SECTION("Maps control and navigation keys")
    {
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_SPACE) == KeyCode::Space);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_RETURN) == KeyCode::Enter);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_ESCAPE) == KeyCode::Escape);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_TAB) == KeyCode::Tab);

        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_UP) == KeyCode::Up);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_DOWN) == KeyCode::Down);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_LEFT) == KeyCode::Left);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_RIGHT) == KeyCode::Right);

        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_PAGEUP) == KeyCode::PageUp);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_PAGEDOWN) == KeyCode::PageDown);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_HOME) == KeyCode::Home);
    }

    SECTION("Unmapped or unknown scancodes return Unknown")
    {
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_UNKNOWN) == KeyCode::Unknown);
        REQUIRE(SDL3KeyMap::toKeyCode(SDL_SCANCODE_COUNT) == KeyCode::Unknown);
    }
}
