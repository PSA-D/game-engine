#pragma once

#include "utils/Color.h"
#include "utils/Easing.h"
#include "utils/Vector2.h"

namespace engine
{
    class Interpolator
    {
    public:
        [[nodiscard]] static float lerp(float start, float end, float t);
        [[nodiscard]] static Vector2 lerp(const Vector2& start, const Vector2& end, float t);
        [[nodiscard]] static Color lerp(const Color& start, const Color& end, float t);
        [[nodiscard]] static float ease(Easing easing, float t);
    };
} // namespace engine
