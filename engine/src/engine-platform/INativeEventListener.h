#pragma once

namespace engine::platform
{
    class INativeEventListener
    {
    public:
        virtual ~INativeEventListener() = default;
        virtual bool onNativeEvent(const void* nativeEvent) = 0;
    };
}