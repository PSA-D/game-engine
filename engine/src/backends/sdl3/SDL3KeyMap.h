#pragma once

#include "platform/Events.h"
#include "utils/InputCodes.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

namespace engine::backends
{
// Veranderd een SDL input naar een KeyCode of MouseButton
class SDL3KeyMap
{
public:
    SDL3KeyMap() = default;
    static KeyCode toKeyCode(SDL_Scancode scancode);
    static platform::KeyModifiers toKeyModifiers(SDL_Keymod modifier);
    static MouseButton toMouseButton(uint8_t button);
};
}; // namespace engine::backends

