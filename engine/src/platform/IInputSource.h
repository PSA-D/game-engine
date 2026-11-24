#pragma once

#include "Events.h"
#include "INativeEventListener.h"

#include <optional>

namespace engine::platform
{
    // De bron van de PlatformEvent's. Met `pollEvent()` kan je een event ophalen.
    // Er kunnen INativeEventListeners worden toegevoegd voor de rauwe events.
    class IInputSource
    {
    public:
        virtual ~IInputSource() = default;
        virtual std::optional<PlatformEvent> pollEvent() = 0;
        virtual void addNativeEventListener(INativeEventListener* listener) = 0;
        virtual void removeNativeEventListener(INativeEventListener* listener) = 0;
    };
}; // namespace engine::platform
