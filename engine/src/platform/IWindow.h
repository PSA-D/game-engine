#pragma once

#include "utils/Vector2.h"

namespace engine::platform
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;
        virtual Vector2 getSize() const = 0;
    };
} // namespace engine::platform
