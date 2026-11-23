#pragma once

#include "platform/Events.h"
#include "utils/InputCodes.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

namespace backend::sdl3
{
class SDL3KeyMap
{
public:
    SDL3KeyMap() = default;
    static engine::utils::KeyCode toKeyCode(SDL_Scancode scancode);
    static engine::platform::KeyModifiers toKeyModifiers(SDL_Keymod modifier);
    static engine::utils::MouseButton toMouseButton(uint8_t button);
};
}; // namespace backend::sdl3

