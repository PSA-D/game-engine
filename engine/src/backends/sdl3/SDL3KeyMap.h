#pragma once
#include "../../engine-platform/EventKinds.h"
#include "../../engine-platform/Events.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

namespace backend::sdl3
{
    class SDL3KeyMap
    {
    public:
        SDL3KeyMap() = default;
        static engine::platform::KeyCode toKeyCode(SDL_Scancode scancode);
        static engine::platform::KeyModifiers toKeyModifiers(SDL_Keymod modifier);
        static engine::platform::MouseButton toMouseButton(uint8_t button);
    };
};

