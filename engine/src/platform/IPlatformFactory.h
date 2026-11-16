//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IPLATFORMFACTORY_H
#define PROJECTSYSTEMARCHITECTURE_IPLATFORMFACTORY_H

#pragma once

#include "platform/IAudioBackend.h"
#include "platform/IInputSource.h"
#include "platform/IPlatformPaths.h"
#include "platform/IRenderer.h"
#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

#include <memory>
#include <string>

namespace engine::platform
{
// Maakt alle platformonderdelen van één backend (SDL3, of headless).
// Dit is de enige plek waar een backend gekozen wordt.
// Eigenaarschap en levensduur:
// - De factory geeft alles terug als unique_ptr: de aanroeper bezit het resultaat.
// - De factory moet langer leven dan alles wat hij heeft aangemaakt.
// - Aanmaakvolgorde: factory, window, renderer, input source, audio.
//   Vernietigingsvolgorde: omgekeerd (audio, input source, renderer, window, factory).
// - createRenderer() en createInputSource() accepteren alleen een window van deze factory.
// - De vsync-instelling komt uit de WindowConfig van het window.
class IPlatformFactory
{
public:
    IPlatformFactory() = default;
    virtual ~IPlatformFactory() = default;
    IPlatformFactory(const IPlatformFactory&) = delete;
    auto operator=(const IPlatformFactory&) -> IPlatformFactory& = delete;
    IPlatformFactory(IPlatformFactory&&) = delete;
    auto operator=(IPlatformFactory&&) -> IPlatformFactory& = delete;

    [[nodiscard]] virtual std::unique_ptr<IWindow> createWindow(const WindowConfig& config) = 0;
    [[nodiscard]] virtual std::unique_ptr<IRenderer> createRenderer(IWindow& window) = 0;
    [[nodiscard]] virtual std::unique_ptr<IInputSource> createInputSource(IWindow& window) = 0;
    [[nodiscard]] virtual std::unique_ptr<IAudioBackend> createAudioBackend() = 0;
    [[nodiscard]] virtual std::unique_ptr<IPlatformPaths> createPaths(const std::string& appName) = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IPLATFORMFACTORY_H
