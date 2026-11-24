#pragma once

#include <cstdint>

namespace engine
{
    enum class MouseButton : uint8_t
    {
        Left,
        Right,
        Middle,
        X1,
        X2,
        Count,
        Unknown,
    };
} // namespace engine
