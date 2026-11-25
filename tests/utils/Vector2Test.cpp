#include "utils/Vector2.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    TEST_CASE("Een nieuwe Vector2 staat op de oorsprong", "[unit][utils]")
    {
        const Vector2 vector;

        CHECK(vector.x == 0.0F);
        CHECK(vector.y == 0.0F);
    }

    TEST_CASE("Vector2 vergelijkt beide componenten", "[unit][utils]")
    {
        CHECK(Vector2{1.0F, 2.0F} == Vector2{1.0F, 2.0F});
        CHECK(Vector2{1.0F, 2.0F} != Vector2{1.0F, 3.0F});
        CHECK(Vector2{1.0F, 2.0F} != Vector2{2.0F, 2.0F});
    }
} // namespace engine
