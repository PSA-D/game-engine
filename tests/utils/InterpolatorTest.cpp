#include "utils/Interpolator.h"

#include "utils/Color.h"
#include "utils/Easing.h"
#include "utils/Vector2.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

namespace engine
{
    namespace
    {
        constexpr double TOLERANCE = 1e-5;
    } // namespace

    TEST_CASE("lerp interpoleert floats", "[unit][utils]")
    {
        CHECK_THAT(Interpolator::lerp(0.0F, 10.0F, 0.25F), WithinAbs(2.5, TOLERANCE));
        CHECK_THAT(Interpolator::lerp(0.0F, 10.0F, 1.5F), WithinAbs(15.0, TOLERANCE));
    }

    TEST_CASE("lerp interpoleert Vector2 per as", "[unit][utils]")
    {
        const Vector2 mid = Interpolator::lerp(Vector2{0.0F, 0.0F}, Vector2{10.0F, 20.0F}, 0.5F);

        CHECK_THAT(mid.x, WithinAbs(5.0, TOLERANCE));
        CHECK_THAT(mid.y, WithinAbs(10.0, TOLERANCE));
    }

    TEST_CASE("lerp interpoleert Color per kanaal en begrenst t", "[unit][utils]")
    {
        CHECK(Interpolator::lerp(Color::BLACK, Color::WHITE, 0.5F) == Color{128, 128, 128, 255});
        CHECK(Interpolator::lerp(Color::BLACK, Color::WHITE, 2.0F) == Color::WHITE);
        CHECK(Interpolator::lerp(Color::BLACK, Color::WHITE, -1.0F) == Color::BLACK);
    }

    TEST_CASE("Elke easing begint op 0 en eindigt op 1", "[unit][utils]")
    {
        const Easing easing = GENERATE(Easing::Linear, Easing::EaseIn, Easing::EaseOut, Easing::EaseInOut);

        CHECK_THAT(Interpolator::ease(easing, 0.0F), WithinAbs(0.0, TOLERANCE));
        CHECK_THAT(Interpolator::ease(easing, 1.0F), WithinAbs(1.0, TOLERANCE));
    }

    TEST_CASE("Easing-curves hebben de verwachte vorm", "[unit][utils]")
    {
        CHECK_THAT(Interpolator::ease(Easing::Linear, 0.5F), WithinAbs(0.5, TOLERANCE));
        CHECK_THAT(Interpolator::ease(Easing::EaseIn, 0.5F), WithinAbs(0.25, TOLERANCE));
        CHECK_THAT(Interpolator::ease(Easing::EaseOut, 0.5F), WithinAbs(0.75, TOLERANCE));
        CHECK_THAT(Interpolator::ease(Easing::EaseInOut, 0.5F), WithinAbs(0.5, TOLERANCE));
    }

    TEST_CASE("ease begrenst t tot het bereik 0 tot 1", "[unit][utils]")
    {
        CHECK_THAT(Interpolator::ease(Easing::Linear, 1.5F), WithinAbs(1.0, TOLERANCE));
        CHECK_THAT(Interpolator::ease(Easing::EaseIn, -0.5F), WithinAbs(0.0, TOLERANCE));
    }
} // namespace engine
