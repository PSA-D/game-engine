#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

#include <memory>

struct SDL_Window;

namespace engine::backends
{
    class SDL3Window final : public platform::IWindow
    {
    public:
        explicit SDL3Window(const platform::WindowConfig& config);

        [[nodiscard]] std::string getTitle() const override;
        [[nodiscard]] Size getSize() const override;
        [[nodiscard]] Size getPixelSize() const override;

        void setTitle(const std::string& title) override;
        void setFullscreen(bool enabled) override;
        void setSize(Size size) override;

        [[nodiscard]] const platform::WindowConfig& getConfig() const;

        [[nodiscard]] SDL_Window* getNativeHandle() const;

    private:
        struct SdlWindowDeleter
        {
            void operator()(SDL_Window* window) const noexcept;
        };

        platform::WindowConfig config;
        std::unique_ptr<SDL_Window, SdlWindowDeleter> window;
    };
} // namespace engine::backends
