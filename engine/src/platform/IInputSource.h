#pragma once

#include "Events.h"
#include "INativeEventListener.h"

#include <optional>

namespace engine::platform
{
    class IInputSource
    {
    public:
        virtual ~IInputSource() = default;
        virtual std::optional<PlatformEvent> pollEvent() = 0;
        virtual void addNativeEventListener(INativeEventListener* listener) = 0;
        virtual void removeNativeEventListener(INativeEventListener* listener) = 0;
    };
}; // namespace engine::platform
