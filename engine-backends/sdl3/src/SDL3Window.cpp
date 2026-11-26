#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

namespace engine::backends
{
    namespace
    {
        auto toSDLWindow(void* window) noexcept -> SDL_Window*
        {
            return static_cast<SDL_Window*>(window);
        }

        auto toSDLWindowFlags(const platform::WindowConfig& config) noexcept -> SDL_WindowFlags
        {
            SDL_WindowFlags flags = 0;
            if (config.fullscreen)
            {
                flags |= SDL_WINDOW_FULLSCREEN;
            }
            if (config.resizable)
            {
                flags |= SDL_WINDOW_RESIZABLE;
            }
            return flags;
        }
    } // namespace

    SDL3Window::SDL3Window(const platform::WindowConfig& windowConfig)
        : config(windowConfig)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw std::runtime_error(SDL_GetError());
        }

        window =
            SDL_CreateWindow(config.title.c_str(), config.size.width, config.size.height, toSDLWindowFlags(config));
        if (window == nullptr)
        {
            const std::string error = SDL_GetError();
            SDL_Quit();
            throw std::runtime_error(error);
        }
    }

    SDL3Window::~SDL3Window()
    {
        SDL_DestroyWindow(toSDLWindow(window));
        SDL_Quit();
    }

    bool SDL3Window::pollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                return false;
            }
        }
        return true;
    }

    std::string SDL3Window::getTitle() const
    {
        const char* title = SDL_GetWindowTitle(toSDLWindow(window));
        if (title == nullptr)
        {
            throw std::runtime_error(SDL_GetError());
        }
        return title;
    }

    Size SDL3Window::getSize() const
    {
        Size size;
        if (!SDL_GetWindowSize(toSDLWindow(window), &size.width, &size.height))
        {
            throw std::runtime_error(SDL_GetError());
        }
        return size;
    }

    Size SDL3Window::getPixelSize() const
    {
        Size size;
        if (!SDL_GetWindowSizeInPixels(toSDLWindow(window), &size.width, &size.height))
        {
            throw std::runtime_error(SDL_GetError());
        }
        return size;
    }

    const platform::WindowConfig& SDL3Window::getConfig() const noexcept
    {
        return config;
    }

    void* SDL3Window::getNativeHandle() const noexcept
    {
        return window;
    }

    void SDL3Window::setTitle(const std::string& title)
    {
        if (!SDL_SetWindowTitle(toSDLWindow(window), title.c_str()))
        {
            throw std::runtime_error(SDL_GetError());
        }
        config.title = title;
    }

    void SDL3Window::setFullscreen(bool enabled)
    {
        if (!SDL_SetWindowFullscreen(toSDLWindow(window), enabled))
        {
            throw std::runtime_error(SDL_GetError());
        }
        config.fullscreen = enabled;
    }

    void SDL3Window::setSize(Size size)
    {
        if (!SDL_SetWindowSize(toSDLWindow(window), size.width, size.height))
        {
            throw std::runtime_error(SDL_GetError());
        }
        config.size = size;
    }
} // namespace engine::backends
