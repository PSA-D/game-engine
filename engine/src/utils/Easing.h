#pragma once

#include <cstdint>

namespace engine
{
enum class Easing : std::uint8_t
{
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
};
} // namespace engine
