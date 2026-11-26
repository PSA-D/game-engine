#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

namespace engine::backends
{
    class SDL3Window final : public platform::IWindow
    {
    public:
        explicit SDL3Window(const platform::WindowConfig& config);
        ~SDL3Window() override;

        SDL3Window(const SDL3Window&) = delete;
        SDL3Window& operator=(const SDL3Window&) = delete;
        SDL3Window(SDL3Window&&) = delete;
        SDL3Window& operator=(SDL3Window&&) = delete;

        [[nodiscard]] bool pollEvents();
        [[nodiscard]] std::string getTitle() const override;
        [[nodiscard]] Size getSize() const override;
        [[nodiscard]] Size getPixelSize() const override;
        [[nodiscard]] const platform::WindowConfig& getConfig() const noexcept;
        [[nodiscard]] void* getNativeHandle() const noexcept;

        void setTitle(const std::string& title) override;
        void setFullscreen(bool enabled) override;
        void setSize(Size size) override;

    private:
        platform::WindowConfig config;
        void* window{nullptr};
    };
} // namespace engine::backends
