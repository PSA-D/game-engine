#include "Engine.h"
#include "EngineConfig.h"

#include <catch2/catch_test_macros.hpp>

auto runGameObjectTests() -> int;

TEST_CASE("GameObject behavior tests pass", "[game-object]")
{
    REQUIRE(runGameObjectTests() == 0);
}

namespace engine
{
    TEST_CASE("Engine met Headless-backend wordt aangemaakt en weer opgeruimd", "[unit][engine]")
    {
        EngineConfig config;
        config.appName = "Test";
        config.platformBackend = PlatformBackend::Headless;

        CHECK_NOTHROW(Engine{config});
    }

    TEST_CASE("EngineConfig heeft standaard een framerate-limiet van 60", "[unit][engine]")
    {
        const EngineConfig config;

        CHECK(config.targetFrameRate == 60);
    }
} // namespace engine
