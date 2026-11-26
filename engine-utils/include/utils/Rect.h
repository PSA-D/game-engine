#pragma once

#include "utils/Vector2.h"

namespace engine
{
    struct Rect
    {
        Vector2 position;
        Vector2 size;

        [[nodiscard]] bool contains(const Vector2& point) const;
        [[nodiscard]] bool contains(const Rect& other) const;
        [[nodiscard]] bool intersects(const Rect& other) const;

        friend constexpr bool operator==(const Rect&, const Rect&) = default;
    };
} // namespace engine
