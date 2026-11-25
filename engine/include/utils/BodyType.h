#pragma once

#include <cstdint>

namespace engine
{
    enum class BodyType : std::uint8_t
    {
        Static,
        Dynamic,
        Kinematic,
    };
} // namespace engine
