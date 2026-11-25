#pragma once

#include "EngineConfig.h"
#include "platform/IPlatformFactory.h"
#include "platform/WindowConfig.h"

#include <memory>

namespace engine::core
{
    // Vrije functies van de composition root, los van EngineImpl zodat ze te testen zijn.

    // Stelt de WindowConfig op uit de EngineConfig (appName wordt de titel).
    [[nodiscard]] platform::WindowConfig makeWindowConfig(const EngineConfig& config);

    // Kiest de platform-factory voor de gevraagde backend.
    [[nodiscard]] std::unique_ptr<platform::IPlatformFactory> createPlatformFactory(PlatformBackend backend);
} // namespace engine::core
