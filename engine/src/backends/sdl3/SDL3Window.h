#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"

#include <memory>

struct SDL_Window;

namespace engine::backends
{
    // Een echt venster via SDL3.
    // Vereist dat SDL3PlatformFactory (die SDL_Init doet) langer leeft dan dit venster.
    class SDL3Window final : public platform::IWindow
    {
    public:
        explicit SDL3Window(const platform::WindowConfig& config);

        // Leest het echte SDL-venster uit, niet de config.
        [[nodiscard]] std::string getTitle() const override;
        [[nodiscard]] Size getSize() const override;
        [[nodiscard]] Size getPixelSize() const override;

        void setTitle(const std::string& title) override;
        void setFullscreen(bool enabled) override;
        void setSize(Size size) override;

        // De instellingen waarmee het venster gemaakt is; de renderer leest hier straks vsync uit.
        [[nodiscard]] const platform::WindowConfig& getConfig() const;

        // Alleen voor andere SDL3-backendklassen (input source, renderer). Niet opslaan buiten hun levensduur.
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
