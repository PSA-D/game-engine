#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <stdexcept>

namespace engine::backends
{
SDL3Window::SDL3Window(const std::string& title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        const std::string error = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error(error);
    }
}

SDL3Window::~SDL3Window()
{
    SDL_DestroyWindow(static_cast<SDL_Window*>(window));
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

void* SDL3Window::getNativeHandle() const noexcept
{
    return window;
}
} // namespace engine::backends
