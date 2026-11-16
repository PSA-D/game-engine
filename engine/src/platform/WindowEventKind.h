//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_WINDOWEVENTKIND_H
#define PROJECTSYSTEMARCHITECTURE_WINDOWEVENTKIND_H

#pragma once

#include <cstdint>

namespace engine::platform
{
enum class WindowEventKind : std::uint8_t
{
    Resized,
    FocusGained,
    FocusLost,
    Minimized,
    Restored,
    CloseRequested,
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_WINDOWEVENTKIND_H
