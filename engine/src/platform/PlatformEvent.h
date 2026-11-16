//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_PLATFORMEVENT_H
#define PROJECTSYSTEMARCHITECTURE_PLATFORMEVENT_H

#pragma once

#include "platform/KeyModifiers.h"
#include "platform/WindowEventKind.h"
#include "utils/KeyCode.h"
#include "utils/MouseButton.h"
#include "utils/Size.h"

#include <variant>

namespace engine::platform
{
struct KeyEvent
{
    utils::KeyCode key{};
    bool pressed{false};
    bool repeat{false};
    KeyModifiers mods{};
};

struct MouseButtonEvent
{
    utils::MouseButton button{};
    bool pressed{false};
    float x{0.0F};
    float y{0.0F};
};

struct MouseMoveEvent
{
    float x{0.0F};
    float y{0.0F};
    float dx{0.0F};
    float dy{0.0F};
};

struct MouseWheelEvent
{
    float x{0.0F}; // horizontaal
    float y{0.0F}; // verticaal
};

struct WindowEvent
{
    WindowEventKind kind{WindowEventKind::Resized};
    utils::Size size{};
};

struct QuitEvent
{
};

using PlatformEvent = std::variant<KeyEvent, MouseButtonEvent, MouseMoveEvent, MouseWheelEvent,
                                   WindowEvent, QuitEvent>;
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_PLATFORMEVENT_H
