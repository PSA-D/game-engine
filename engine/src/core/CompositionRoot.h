#pragma once

#include "EngineConfig.h"
#include "platform/IPlatformFactory.h"
#include "platform/WindowConfig.h"

#include <memory>

namespace engine::core
{
    // Stelt de WindowConfig op uit de EngineConfig (appName wordt de titel).
    [[nodiscard]] platform::WindowConfig makeWindowConfig(const EngineConfig& config);

    // Kiest de platform-factory voor de gevraagde backend.
    [[nodiscard]] std::unique_ptr<platform::IPlatformFactory> createPlatformFactory(PlatformBackend backend);
} // namespace engine::core
