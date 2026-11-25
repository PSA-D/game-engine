#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

namespace engine::backends
{
    namespace
    {
        SDL_WindowFlags toSdlFlags(const platform::WindowConfig& config)
        {
            SDL_WindowFlags flags = 0;

            if (config.fullscreen)
                flags |= SDL_WINDOW_FULLSCREEN;
            if (config.resizable)
                flags |= SDL_WINDOW_RESIZABLE;

            return flags;
        }
    } // namespace

    void SDL3Window::SdlWindowDeleter::operator()(SDL_Window* window) const noexcept
    {
        SDL_DestroyWindow(window);
    }

    SDL3Window::SDL3Window(const platform::WindowConfig& config)
        : config(config)
        , window(SDL_CreateWindow(config.title.c_str(), config.size.width, config.size.height, toSdlFlags(config)))
    {
        if (window == nullptr)
            throw std::runtime_error(std::string{"SDL_CreateWindow mislukt: "} + SDL_GetError());
    }

    std::string SDL3Window::getTitle() const
    {
        return SDL_GetWindowTitle(window.get());
    }

    Size SDL3Window::getSize() const
    {
        Size size;
        SDL_GetWindowSize(window.get(), &size.width, &size.height);
        return size;
    }

    Size SDL3Window::getPixelSize() const
    {
        Size size;
        SDL_GetWindowSizeInPixels(window.get(), &size.width, &size.height);
        return size;
    }

    void SDL3Window::setTitle(const std::string& title)
    {
        SDL_SetWindowTitle(window.get(), title.c_str());
    }

    void SDL3Window::setFullscreen(bool enabled)
    {
        SDL_SetWindowFullscreen(window.get(), enabled);
    }

    void SDL3Window::setSize(Size size)
    {
        SDL_SetWindowSize(window.get(), size.width, size.height);
    }

    const platform::WindowConfig& SDL3Window::getConfig() const
    {
        return config;
    }

    SDL_Window* SDL3Window::getNativeHandle() const
    {
        return window.get();
    }
} // namespace engine::backends
