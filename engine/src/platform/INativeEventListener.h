#pragma once

namespace engine::platform
{
    // Een event listener voor rauwe events (zoals SDL Events), niet PlatformEvent's
    class INativeEventListener
    {
    public:
        virtual ~INativeEventListener() = default;
        virtual bool onNativeEvent(const void* nativeEvent) = 0;
    };
} // namespace engine::platform
