// Laag: UNIT. Alles in het geheugen.

#include "src/runtime/Level.h"

#include "src/runtime/Region.h"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

namespace engine
{
TEST_CASE("Een nieuw Level heeft een naam en geen Regions", "[unit][runtime]")
{
    const Level level("Level 1");

    CHECK(level.getName() == "Level 1");
    CHECK(level.getRegions().empty());
    CHECK(level.getActiveRegion() == nullptr);
}

TEST_CASE("createRegion maakt een Region met die naam", "[unit][runtime]")
{
    Level level("Level 1");

    auto& region = level.createRegion("Start");

    CHECK(region.getName() == "Start");
    REQUIRE(level.getRegions().size() == 1);
    CHECK(level.getRegions().front() == &region);
}

TEST_CASE("Regions staan in volgorde van aanmaken en namen mogen dubbel zijn", "[unit][runtime]")
{
    Level level("Level 1");
    auto& first = level.createRegion("Kamer");
    auto& second = level.createRegion("Kamer");

    const auto regions = level.getRegions();

    REQUIRE(regions.size() == 2);
    CHECK(regions[0] == &first);
    CHECK(regions[1] == &second);
}

TEST_CASE("De const-versie van getRegions geeft dezelfde Regions", "[unit][runtime]")
{
    Level level("Level 1");
    auto& region = level.createRegion("Start");
    const Level& view = level;

    const auto regions = view.getRegions();

    REQUIRE(regions.size() == 1);
    CHECK(regions.front() == &region);
}

TEST_CASE("Een nieuwe Region is niet vanzelf actief", "[unit][runtime]")
{
    Level level("Level 1");
    level.createRegion("Start");

    CHECK(level.getActiveRegion() == nullptr);
}

TEST_CASE("activateRegion maakt een eigen Region actief", "[unit][runtime]")
{
    Level level("Level 1");
    auto& first = level.createRegion("Een");
    auto& second = level.createRegion("Twee");

    CHECK(level.activateRegion(first));
    CHECK(level.getActiveRegion() == &first);

    CHECK(level.activateRegion(second));
    CHECK(level.getActiveRegion() == &second);
}

TEST_CASE("activateRegion weigert een Region van een ander Level", "[unit][runtime]")
{
    Level level("Level 1");
    auto& own = level.createRegion("Eigen");
    level.activateRegion(own);
    Level other("Level 2");
    auto& foreign = other.createRegion("Vreemd");

    CHECK_FALSE(level.activateRegion(foreign));
    CHECK(level.getActiveRegion() == &own); // niets veranderd
}
} // namespace engine
