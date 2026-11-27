// Laag: UNIT. De facade zonder draaiende engine: er is geen subsysteem gekoppeld, dus ook
// geen bestanden. Elke aanroep moet netjes mislukken.

#include "core/Persistence.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    TEST_CASE("Zonder draaiende engine mislukt elke aanroep netjes", "[unit][persistence]")
    {
        const Level level("Level");

        CHECK_FALSE(Persistence::saveGame("slot1", DataNode(1)));
        CHECK(Persistence::getLastError() == PersistenceError::NotAvailable);
        CHECK_FALSE(Persistence::loadGame("slot1").has_value());
        CHECK_FALSE(Persistence::hasSave("slot1"));
        CHECK_FALSE(Persistence::deleteSave("slot1"));
        CHECK(Persistence::listSaves().empty());
        CHECK_FALSE(Persistence::saveLevel("level1", level));
        CHECK(Persistence::loadLevel("level1") == nullptr);
    }

    TEST_CASE("De facade geeft foutteksten voor game-code", "[unit][persistence]")
    {
        CHECK_FALSE(Persistence::describeError(PersistenceError::NotFound).empty());
        CHECK_FALSE(Persistence::describeError(Persistence::getLastError()).empty());
    }
} // namespace engine