#pragma once

#include "SDL3Window.h"
#include "platform/Events.h"
#include "platform/IInputSource.h"
#include "platform/INativeEventListener.h"

#include <SDL3/SDL.h>
#include <optional>
#include <vector>

namespace engine::backends
{
    // Een SDL3 implementatie van de IInputSource
    class SDL3InputSource : public platform::IInputSource
    {
    public:
        SDL3InputSource(SDL3Window* window)
            : window_(window)
        {
        }
        std::optional<platform::PlatformEvent> pollEvent() override;
        void addNativeEventListener(platform::INativeEventListener* listener) override;
        void removeNativeEventListener(platform::INativeEventListener* listener) override;

    private:
        // Veranderd SDL3 events naar PlatformEvent's
        std::optional<platform::PlatformEvent> translateEvent(const SDL_Event& event) const;

        SDL3Window* window_;
        std::vector<platform::INativeEventListener*> listeners_{};
    };
} // namespace engine::backends
