#pragma once

#include "platform/IPlatformFactory.h"

namespace engine::backends
{
    // Factory voor de SDL3-backend.
    // - Constructor: SDL_Init. Destructor: SDL_Quit.
    // - Moet daarom als eerste aangemaakt en als laatste vernietigd worden.
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
    };
} // namespace engine::backends
