#pragma once

#include "EventKinds.h"
#include "utils/Vector2.h"

#include <variant>

namespace engine::platform
{
struct WindowEvent
{
    WindowEventKind kind;
    utils::Vector2 size;
};

struct QuitEvent
{
};

struct MouseButtonEvent
{
    MouseButton button = MouseButton::Unknown;
    bool pressed = false;
    float x = 0.0f;
    float y = 0.0f;
};

struct MouseMoveEvent
{
    float x = 0.0f;
    float y = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;
};

struct MouseWheelEvent
{
    float x = 0.0f;
    float y = 0.0f;
};

struct KeyModifiers
{
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool gui = false;
    bool capslock = false;
    bool numlock = false;
};

struct KeyEvent
{
    KeyCode key = KeyCode::Unknown;
    KeyModifiers mods;
    bool pressed = false;
    bool repeat = false;
};

using PlatformEvent = std::variant<WindowEvent, MouseWheelEvent, QuitEvent, MouseMoveEvent, MouseButtonEvent, KeyEvent>;

} // namespace engine::platform
