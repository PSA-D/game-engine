#include "utils/Rect.h"

#include "utils/Vector2.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    TEST_CASE("Rect bevat punten binnen zijn randen", "[unit][utils]")
    {
        const Rect rect{{0.0F, 0.0F}, {10.0F, 10.0F}};

        CHECK(rect.contains(Vector2{5.0F, 5.0F}));
        CHECK(rect.contains(Vector2{0.0F, 0.0F}));
        CHECK_FALSE(rect.contains(Vector2{10.0F, 5.0F}));
        CHECK_FALSE(rect.contains(Vector2{5.0F, 10.0F}));
        CHECK_FALSE(rect.contains(Vector2{-1.0F, 5.0F}));
    }

    TEST_CASE("Rect bevat alleen rechthoeken die er volledig binnen vallen", "[unit][utils]")
    {
        const Rect rect{{0.0F, 0.0F}, {10.0F, 10.0F}};

        CHECK(rect.contains(Rect{{2.0F, 2.0F}, {8.0F, 8.0F}}));
        CHECK(rect.contains(rect));
        CHECK_FALSE(rect.contains(Rect{{5.0F, 5.0F}, {10.0F, 10.0F}}));
    }

    TEST_CASE("Rect snijdt alleen bij echte overlap", "[unit][utils]")
    {
        const Rect rect{{0.0F, 0.0F}, {10.0F, 10.0F}};

        CHECK(rect.intersects(Rect{{5.0F, 5.0F}, {10.0F, 10.0F}}));
        CHECK_FALSE(rect.intersects(Rect{{10.0F, 0.0F}, {5.0F, 5.0F}}));
        CHECK_FALSE(rect.intersects(Rect{{20.0F, 20.0F}, {5.0F, 5.0F}}));
    }
} // namespace engine
