#include "SDL3KeyMap.h"

#include <SDL3/SDL_mouse.h>

using namespace engine::platform;

namespace engine::backends
{
KeyModifiers SDL3KeyMap::toKeyModifiers(SDL_Keymod modifier)
{
    return {.shift = (modifier & SDL_KMOD_SHIFT) != 0,
            .ctrl = (modifier & SDL_KMOD_CTRL) != 0,
            .alt = (modifier & SDL_KMOD_ALT) != 0,
            .gui = (modifier & SDL_KMOD_GUI) != 0,
            .capslock = (modifier & SDL_KMOD_CAPS) != 0,
            .numlock = (modifier & SDL_KMOD_NUM) != 0};
}

MouseButton SDL3KeyMap::toMouseButton(uint8_t button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return MouseButton::Left;
    case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
    case SDL_BUTTON_X1:
        return MouseButton::X1;
    case SDL_BUTTON_X2:
        return MouseButton::X2;
    default:
        return MouseButton::Unknown;
    }
}

KeyCode SDL3KeyMap::toKeyCode(SDL_Scancode scancode)
{
    switch (scancode)
    {
    case SDL_SCANCODE_A:
        return KeyCode::A;
    case SDL_SCANCODE_B:
        return KeyCode::B;
    case SDL_SCANCODE_C:
        return KeyCode::C;
    case SDL_SCANCODE_D:
        return KeyCode::D;
    case SDL_SCANCODE_E:
        return KeyCode::E;
    case SDL_SCANCODE_F:
        return KeyCode::F;
    case SDL_SCANCODE_G:
        return KeyCode::G;
    case SDL_SCANCODE_H:
        return KeyCode::H;
    case SDL_SCANCODE_I:
        return KeyCode::I;
    case SDL_SCANCODE_J:
        return KeyCode::J;
    case SDL_SCANCODE_K:
        return KeyCode::K;
    case SDL_SCANCODE_L:
        return KeyCode::L;
    case SDL_SCANCODE_M:
        return KeyCode::M;
    case SDL_SCANCODE_N:
        return KeyCode::N;
    case SDL_SCANCODE_O:
        return KeyCode::O;
    case SDL_SCANCODE_P:
        return KeyCode::P;
    case SDL_SCANCODE_Q:
        return KeyCode::Q;
    case SDL_SCANCODE_R:
        return KeyCode::R;
    case SDL_SCANCODE_S:
        return KeyCode::S;
    case SDL_SCANCODE_T:
        return KeyCode::T;
    case SDL_SCANCODE_U:
        return KeyCode::U;
    case SDL_SCANCODE_V:
        return KeyCode::V;
    case SDL_SCANCODE_W:
        return KeyCode::W;
    case SDL_SCANCODE_X:
        return KeyCode::X;
    case SDL_SCANCODE_Y:
        return KeyCode::Y;
    case SDL_SCANCODE_Z:
        return KeyCode::Z;

    case SDL_SCANCODE_0:
        return KeyCode::Num0;
    case SDL_SCANCODE_1:
        return KeyCode::Num1;
    case SDL_SCANCODE_2:
        return KeyCode::Num2;
    case SDL_SCANCODE_3:
        return KeyCode::Num3;
    case SDL_SCANCODE_4:
        return KeyCode::Num4;
    case SDL_SCANCODE_5:
        return KeyCode::Num5;
    case SDL_SCANCODE_6:
        return KeyCode::Num6;
    case SDL_SCANCODE_7:
        return KeyCode::Num7;
    case SDL_SCANCODE_8:
        return KeyCode::Num8;
    case SDL_SCANCODE_9:
        return KeyCode::Num9;

    case SDL_SCANCODE_SPACE:
        return KeyCode::Space;
    case SDL_SCANCODE_RETURN:
        return KeyCode::Enter;
    case SDL_SCANCODE_ESCAPE:
        return KeyCode::Escape;
    case SDL_SCANCODE_TAB:
        return KeyCode::Tab;
    case SDL_SCANCODE_BACKSPACE:
        return KeyCode::Backspace;

    case SDL_SCANCODE_UP:
        return KeyCode::ArrowUp;
    case SDL_SCANCODE_DOWN:
        return KeyCode::ArrowDown;
    case SDL_SCANCODE_LEFT:
        return KeyCode::ArrowLeft;
    case SDL_SCANCODE_RIGHT:
        return KeyCode::ArrowRight;

    case SDL_SCANCODE_PAGEUP:
        return KeyCode::PageUp;
    case SDL_SCANCODE_PAGEDOWN:
        return KeyCode::PageDown;
    case SDL_SCANCODE_HOME:
        return KeyCode::Home;
    case SDL_SCANCODE_END:
        return KeyCode::End;

    case SDL_SCANCODE_F1:
        return KeyCode::F1;
    case SDL_SCANCODE_F2:
        return KeyCode::F2;
    case SDL_SCANCODE_F3:
        return KeyCode::F3;
    case SDL_SCANCODE_F4:
        return KeyCode::F4;
    case SDL_SCANCODE_F5:
        return KeyCode::F5;
    case SDL_SCANCODE_F6:
        return KeyCode::F6;
    case SDL_SCANCODE_F7:
        return KeyCode::F7;
    case SDL_SCANCODE_F8:
        return KeyCode::F8;
    case SDL_SCANCODE_F9:
        return KeyCode::F9;
    case SDL_SCANCODE_F10:
        return KeyCode::F10;
    case SDL_SCANCODE_F11:
        return KeyCode::F11;
    case SDL_SCANCODE_F12:
        return KeyCode::F12;

    default:
        return KeyCode::Unknown;
    }
}
} // namespace engine::backends
