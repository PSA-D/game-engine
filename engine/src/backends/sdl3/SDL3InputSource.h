#pragma once

#include "SDL3Window.h"
#include "../../engine-platform/IInputSource.h"
#include "../../engine-platform/INativeEventListener.h"
#include "../../engine-platform/Events.h"

#include <SDL3/SDL.h>

#include <vector>
#include <optional>

namespace backend::sdl3 {
    class SDL3InputSource : public engine::platform::IInputSource
    {
    public:
        SDL3InputSource(SDL3Window& window) : window_(window) {}
        std::optional<engine::platform::PlatformEvent> pollEvent() override;
        void addNativeEventListener(engine::platform::INativeEventListener* listener) override;
        void removeNativeEventListener(engine::platform::INativeEventListener* listener) override;
    private:
        std::optional<engine::platform::PlatformEvent> translateEvent(const SDL_Event& event) const;

        SDL3Window& window_;
        std::vector<engine::platform::INativeEventListener*> listeners_{};
    };
}
