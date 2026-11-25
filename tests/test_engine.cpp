#include "Engine.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Engine is not running before init", "[engine]")
{
    engine::Engine engine;

    REQUIRE(engine.isRunning() == false);
}

TEST_CASE("Engine is running after init", "[engine]")
{
    engine::Engine engine;

    engine.init();

    REQUIRE(engine.isRunning() == true);
}

TEST_CASE("Engine is not running after shutdown", "[engine]")
{
    engine::Engine engine;

    engine.init();
    engine.shutdown();

    REQUIRE(engine.isRunning() == false);
}
