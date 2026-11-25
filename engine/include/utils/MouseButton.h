#pragma once

#include <cstdint>

namespace engine
{
    enum class MouseButton : std::uint8_t
    {
        Unknown = 0,

        Left,
        Right,
        Middle,
        Button4,
        Button5,
    };
} // namespace engine
