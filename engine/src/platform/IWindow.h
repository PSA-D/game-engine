//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IWINDOW_H
#define PROJECTSYSTEMARCHITECTURE_IWINDOW_H

#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
// Een window waarin het spel getoond wordt.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IPlatformFactory::createWindow(). De aanroeper bezit hem.
// - Moet vernietigd worden ná de renderer en de input source die ermee gemaakt zijn.
class IWindow
{
public:
    IWindow() = default;
    virtual ~IWindow() = default;
    IWindow(const IWindow&) = delete;
    auto operator=(const IWindow&) -> IWindow& = delete;
    IWindow(IWindow&&) = delete;
    auto operator=(IWindow&&) -> IWindow& = delete;

    [[nodiscard]] virtual utils::Size getSize() const = 0;      // in schermeenheden
    [[nodiscard]] virtual utils::Size getPixelSize() const = 0; // in pixels (anders bij high-DPI)
    virtual void setTitle(const std::string& title) = 0;
    virtual void setFullscreen(bool enabled) = 0;
    virtual void setSize(utils::Size size) = 0; // in schermeenheden
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IWINDOW_H
