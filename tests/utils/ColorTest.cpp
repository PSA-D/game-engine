#include "utils/Color.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    TEST_CASE("Color-constanten zijn volledig dekkend", "[unit][utils]")
    {
        CHECK(Color::WHITE == Color{255, 255, 255, 255});
        CHECK(Color::BLACK == Color{0, 0, 0, 255});
    }

    TEST_CASE("Een nieuwe Color is standaard dekkend", "[unit][utils]")
    {
        CHECK(Color{}.a == 255);
        CHECK(Color{10, 20, 30}.a == 255);
    }
} // namespace engine
