// Laag: INTEGRATIE. Game-code gebruikt alleen de Persistence-facade. Eronder draait een echt
// PersistenceSubsystem met JsonSaveStrategy en echte bestanden, zoals de engine dat koppelt.

#include "core/Persistence.h"
#include "persistence/JsonSaveStrategy.h"
#include "persistence/PersistenceSubsystem.h"
#include "core/ComponentRegistry.h"
#include "LevelTestData.h"
#include "PersistenceTestData.h"

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace engine
{
// Alleen voor tests: koppelt de facade aan een subsysteem, zoals EngineImpl dat doet.
class PersistenceTestAccess
{
public:
    static void bind(subsystems::PersistenceSubsystem* subsystem) { Persistence::setSystem(subsystem); }
};

namespace
{
namespace test = subsystems;

// Een draaiende "engine": subsysteem op een tijdelijke map, aan de facade gekoppeld.
// Ontkoppelt weer aan het eind, zodat tests elkaar niet beïnvloeden.
struct RunningEngine
{
    subsystems::TestPaths paths;
    ComponentRegistry registry;
    subsystems::PersistenceSubsystem subsystem;

    RunningEngine() : subsystem(std::make_unique<subsystems::JsonSaveStrategy>(), paths, registry)
    {
        subsystems::registerTestComponents(registry);
        PersistenceTestAccess::bind(&subsystem);
    }
    ~RunningEngine() { PersistenceTestAccess::bind(nullptr); }
    RunningEngine(const RunningEngine&) = delete;
    auto operator=(const RunningEngine&) -> RunningEngine& = delete;
    RunningEngine(RunningEngine&&) = delete;
    auto operator=(RunningEngine&&) -> RunningEngine& = delete;
};

auto makeSaveData() -> DataNode
{
    auto items = DataNode::makeArray();
    items.pushToArray(DataNode("sword"));
    auto save = DataNode::makeObject();
    save.set("score", DataNode(120));
    save.set("naam", DataNode("Hero"));
    save.set("items", std::move(items));
    return save;
}
} // namespace

TEST_CASE("Game-code kan een savegame opslaan en laden via de facade", "[integration][persistence]")
{
    const RunningEngine running;
    const auto save = makeSaveData();

    REQUIRE(Persistence::saveGame("slot1", save));
    CHECK(Persistence::hasSave("slot1"));
    const auto loaded = Persistence::loadGame("slot1");

    REQUIRE(loaded.has_value());
    CHECK(*loaded == save);
    CHECK(loaded->get("score")->tryInt() == 120);
    CHECK(Persistence::getLastError() == PersistenceError::None);
}

TEST_CASE("De facade meldt de reden van een fout", "[integration][persistence]")
{
    const RunningEngine running;

    SECTION("Een ontbrekend bestand")
    {
        CHECK_FALSE(Persistence::loadGame("bestaat-niet").has_value());
        CHECK(Persistence::getLastError() == PersistenceError::NotFound);
    }
    SECTION("Een ongeldige naam")
    {
        CHECK_FALSE(Persistence::saveGame("../evil", makeSaveData()));
        CHECK(Persistence::getLastError() == PersistenceError::InvalidName);
    }
    SECTION("Een beschadigd bestand")
    {
        test::writeFileWithFolders(running.paths.getUserDataPath() / "saves" / "kapot.json",
                                   "geen json");

        CHECK_FALSE(Persistence::loadGame("kapot").has_value());
        CHECK(Persistence::getLastError() == PersistenceError::InvalidData);
    }
    SECTION("Na een succes is de fout weer leeg")
    {
        CHECK_FALSE(Persistence::loadGame("bestaat-niet").has_value());
        REQUIRE(Persistence::saveGame("slot1", makeSaveData()));
        CHECK(Persistence::getLastError() == PersistenceError::None);
    }

    CHECK_FALSE(Persistence::describeError(Persistence::getLastError()).empty());
}

TEST_CASE("Slots tonen en verwijderen via de facade", "[integration][persistence]")
{
    const RunningEngine running;
    REQUIRE(Persistence::saveGame("b", makeSaveData()));
    REQUIRE(Persistence::saveGame("a", makeSaveData()));

    CHECK(Persistence::listSaves() == std::vector<std::string>{"a", "b"});

    CHECK(Persistence::deleteSave("a"));
    CHECK_FALSE(Persistence::hasSave("a"));
    CHECK(Persistence::listSaves() == std::vector<std::string>{"b"});
}

TEST_CASE("Een level opslaan en laden via de facade", "[integration][persistence]")
{
    const RunningEngine running;
    const auto original = test::buildSampleLevel();

    REQUIRE(Persistence::saveLevel("level1", *original));
    const auto loaded = Persistence::loadLevel("level1");

    REQUIRE(loaded != nullptr);
    CHECK(test::snapshot(*loaded) == test::snapshot(*original));
}

TEST_CASE("Na het stoppen van de engine mislukt de facade weer netjes", "[integration][persistence]")
{
    {
        const RunningEngine running;
        REQUIRE(Persistence::saveGame("slot1", makeSaveData()));
    }

    CHECK_FALSE(Persistence::loadGame("slot1").has_value());
    CHECK(Persistence::getLastError() == PersistenceError::NotAvailable);
}
} // namespace engine