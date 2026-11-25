#include "utils/Interpolator.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace engine
{
    namespace
    {
        constexpr float MAX_CHANNEL_VALUE = 255.0F;
        constexpr float HALFWAY = 0.5F;
        constexpr float TWO = 2.0F;

        std::uint8_t lerpChannel(std::uint8_t start, std::uint8_t end, float t)
        {
            const float value = std::lerp(static_cast<float>(start), static_cast<float>(end), t);
            return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0F, MAX_CHANNEL_VALUE)));
        }
    } // namespace

    float Interpolator::lerp(float start, float end, float t)
    {
        return std::lerp(start, end, t);
    }

    Vector2 Interpolator::lerp(const Vector2& start, const Vector2& end, float t)
    {
        return Vector2{std::lerp(start.x, end.x, t), std::lerp(start.y, end.y, t)};
    }

    Color Interpolator::lerp(const Color& start, const Color& end, float t)
    {
        const float clamped = std::clamp(t, 0.0F, 1.0F);
        return Color{
            lerpChannel(start.r, end.r, clamped),
            lerpChannel(start.g, end.g, clamped),
            lerpChannel(start.b, end.b, clamped),
            lerpChannel(start.a, end.a, clamped),
        };
    }

    float Interpolator::ease(Easing easing, float t)
    {
        const float x = std::clamp(t, 0.0F, 1.0F);

        switch (easing)
        {
        case Easing::Linear:
            return x;
        case Easing::EaseIn:
            return x * x;
        case Easing::EaseOut:
            return 1.0F - ((1.0F - x) * (1.0F - x));
        case Easing::EaseInOut:
        {
            if (x < HALFWAY)
            {
                return TWO * x * x;
            }
            const float inverted = TWO - (TWO * x);
            return 1.0F - (inverted * inverted / TWO);
        }
        }

        return x;
    }
} // namespace engine
