//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_WINDOWCONFIG_H
#define PROJECTSYSTEMARCHITECTURE_WINDOWCONFIG_H

#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
// Instellingen voor het aanmaken van een venster. De afmetingen worden uitgedrukt in schermeenheden, niet in pixels.
struct WindowConfig
{
    std::string title{"Game"};
    utils::Size size{1280, 720};
    bool fullscreen{false};
    bool resizable{true};
    bool vsync{false};
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_WINDOWCONFIG_H
