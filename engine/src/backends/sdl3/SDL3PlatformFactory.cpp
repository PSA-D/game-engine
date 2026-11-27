#include "backends/sdl3/SDL3PlatformFactory.h"

#include "backends/sdl3/SDL3InputSource.h"
#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

namespace engine::backends
{
    SDL3PlatformFactory::SDL3PlatformFactory()
    {
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

    std::unique_ptr<platform::IInputSource> SDL3PlatformFactory::createInputSource(platform::IWindow& window)
    {
        return std::make_unique<SDL3InputSource>(window);
    }
} // namespace engine::backends
