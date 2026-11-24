#pragma once

#include "utils/KeyCode.h"
#include "utils/MouseButton.h"
#include "utils/Vector2.h"

#include <variant>

namespace engine::platform
{
enum class WindowEventKind
{
    Resized,
    FocusGained,
    FocustLost,
    Minimized,
    Maximized,
    Restored,
    CloseRequested,
};

struct WindowEvent
{
    WindowEventKind kind;
    Vector2 size;
};

struct QuitEvent
{
};

struct MouseButtonEvent
{
    MouseButton button = MouseButton::Unknown;
    bool pressed = false;
    Vector2 position = {0.0f, 0.0f};
};

struct MouseMoveEvent
{
    Vector2 position = {0.0f, 0.0f};
    Vector2 difference = {0.0f, 0.0f};
};

struct MouseWheelEvent
{
    Vector2 scroll = {0.0f, 0.0f};
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
