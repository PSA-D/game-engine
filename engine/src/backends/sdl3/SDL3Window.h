#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"
#include "utils/Vector2.h"

namespace engine::backends
{
    // temp class
    class SDL3Window : public platform::IWindow
    {
    public:
        SDL3Window(platform::WindowConfig config)
            : size_(config.size)
        {
        }
        Vector2 getSize() const override
        {
            return {};
        };

    private:
        Vector2 size_;
    };
} // namespace engine::backends
