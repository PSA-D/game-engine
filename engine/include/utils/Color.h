#pragma once

#include <cstdint>
#include <limits>

namespace engine
{
    struct Color
    {
        std::uint8_t r{0};
        std::uint8_t g{0};
        std::uint8_t b{0};
        std::uint8_t a{std::numeric_limits<std::uint8_t>::max()};

        static const Color WHITE;
        static const Color BLACK;

        friend constexpr bool operator==(const Color&, const Color&) = default;
    };

    inline constexpr Color Color::WHITE{255, 255, 255, 255};
    inline constexpr Color Color::BLACK{0, 0, 0, 255};
} // namespace engine
