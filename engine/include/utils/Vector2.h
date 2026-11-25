#pragma once

namespace engine
{
    struct Vector2
    {
        float x{0.0F};
        float y{0.0F};

        friend constexpr bool operator==(const Vector2&, const Vector2&) = default;
    };
} // namespace engine
