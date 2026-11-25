#pragma once

#include "platform/IWindow.h"
#include "platform/WindowConfig.h"
#include "utils/Size.h"

namespace engine::backends
{
    class SDL3Window : public platform::IWindow
    {
    public:
        SDL3Window(platform::WindowConfig config)
            : size_(config.size)
        {
        }
        Size getSize() const override
        {
            return {};
        };

    private:
        Size size_;
    };
} // namespace engine::backends
