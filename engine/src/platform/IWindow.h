#pragma once

#include "utils/Size.h"

namespace engine::platform
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;
        virtual Size getSize() const = 0;
    };
} // namespace engine::platform
