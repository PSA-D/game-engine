#pragma once

namespace engine::platform
{
    // For raw events, not a PlatformEvent
    class INativeEventListener
    {
    public:
        virtual ~INativeEventListener() = default;
        virtual bool onNativeEvent(const void* nativeEvent) = 0;
    };
}