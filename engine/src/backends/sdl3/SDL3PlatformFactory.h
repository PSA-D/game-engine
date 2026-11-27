#pragma once

#include "platform/IPlatformFactory.h"

namespace engine::backends
{
    class SDL3PlatformFactory final : public platform::IPlatformFactory
    {
    public:
        SDL3PlatformFactory();
        ~SDL3PlatformFactory() override;

        SDL3PlatformFactory(const SDL3PlatformFactory&) = delete;
        auto operator=(const SDL3PlatformFactory&) -> SDL3PlatformFactory& = delete;
        SDL3PlatformFactory(SDL3PlatformFactory&&) = delete;
        auto operator=(SDL3PlatformFactory&&) -> SDL3PlatformFactory& = delete;

        [[nodiscard]] std::unique_ptr<platform::IWindow> createWindow(const platform::WindowConfig& config) override;
        [[nodiscard]] std::unique_ptr<platform::IInputSource> createInputSource(platform::IWindow& window) override;
    };
} // namespace engine::backends
