#include "Engine.h"

#include <catch2/catch_test_macros.hpp>

auto runGameObjectTests() -> int;

auto main() -> int
{
    int failures = runGameObjectTests();
TEST_CASE("Engine lifecycle manages running state", "[engine]")
{
    engine::Engine engine;

    SECTION("Engine is not running before init")
    {
        REQUIRE_FALSE(engine.isRunning());
    }

    SECTION("Engine is running after init")
    {
        engine.init();
        REQUIRE(engine.isRunning());
    }

    SECTION("Engine is not running after shutdown")
    {
        engine.init();
        engine.shutdown();
        REQUIRE_FALSE(engine.isRunning());
    }
}
