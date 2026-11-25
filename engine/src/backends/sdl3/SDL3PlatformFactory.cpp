#include "backends/sdl3/SDL3PlatformFactory.h"

#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

namespace engine::backends
{
    SDL3PlatformFactory::SDL3PlatformFactory()
    {
        // Let op: in SDL3 geeft SDL_Init true terug bij succes (in SDL2 was dat 0).
        if (!SDL_Init(SDL_INIT_VIDEO))
            throw std::runtime_error(std::string{"SDL_Init mislukt: "} + SDL_GetError());
    }

    SDL3PlatformFactory::~SDL3PlatformFactory()
    {
        SDL_Quit();
    }

    std::unique_ptr<platform::IWindow> SDL3PlatformFactory::createWindow(const platform::WindowConfig& config)
    {
        return std::make_unique<SDL3Window>(config);
    }
} // namespace engine::backends
