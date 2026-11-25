// Laag: INTEGRATIE. Werkt samen met het echte bestandssysteem (en de JSON-bibliotheek).
// De logica zelf staat in JsonSaveStrategyTest.cpp. Hier controleren we alleen wat er met
// een echt bestand kan misgaan.

#include "persistence/ISaveStrategy.h"
#include "persistence/JsonSaveStrategy.h"
#include "PersistenceTestData.h"
#include "TempFile.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <system_error>

namespace engine::subsystems
{
namespace
{
using engine::DataNode;

using subsystems::readFile;
using subsystems::TempFile;
using subsystems::writeFile;
} // namespace

TEST_CASE("Data kan naar een bestand geschreven en weer uitgelezen worden",
          "[integration][persistence]")
{
    const JsonSaveStrategy strategy;
    const GameData original = makeSampleData();
    const TempFile file("engine_save_strategy_roundtrip" + strategy.fileExtension());

    writeFile(file.getPath(), strategy.serialize(original));
    REQUIRE(std::filesystem::exists(file.getPath()));

    const auto result = strategy.deserialize(readFile(file.getPath()));

    REQUIRE(result.has_value());
    CHECK(*result == original);
}

TEST_CASE("Tekst met regeleinden overleeft het bestand", "[integration][persistence]")
{
    const JsonSaveStrategy strategy;
    const GameData original{.version = 1, .root = DataNode("regel 1\nregel 2\r\nregel 3 é")};
    const TempFile file("engine_save_strategy_newlines" + strategy.fileExtension());

    writeFile(file.getPath(), strategy.serialize(original));
    const auto result = strategy.deserialize(readFile(file.getPath()));

    REQUIRE(result.has_value());
    CHECK(*result == original);
}

TEST_CASE("Een andere strategie-instantie leest hetzelfde bestand", "[integration][persistence]")
{
    // De strategie heeft geen toestand: wat de ene schrijft, leest de andere (zoals na een herstart).
    const GameData original = makeSampleData();
    const TempFile file("engine_save_strategy_instances.json");

    writeFile(file.getPath(), JsonSaveStrategy{}.serialize(original));
    const auto result = JsonSaveStrategy{}.deserialize(readFile(file.getPath()));

    REQUIRE(result.has_value());
    CHECK(*result == original);
}

TEST_CASE("Een beschadigd bestand op schijf geeft nullopt", "[integration][persistence]")
{
    const JsonSaveStrategy strategy;
    const TempFile file("engine_save_strategy_corrupt" + strategy.fileExtension());

    SECTION("Afgebroken bestand, bijvoorbeeld door een crash tijdens het opslaan")
    {
        const std::string complete = strategy.serialize(makeSampleData());
        writeFile(file.getPath(), complete.substr(0, complete.size() / 2));

        CHECK_FALSE(strategy.deserialize(readFile(file.getPath())).has_value());
    }

    SECTION("Leeg bestand")
    {
        writeFile(file.getPath(), "");

        CHECK_FALSE(strategy.deserialize(readFile(file.getPath())).has_value());
    }
}

TEST_CASE("Een bestand wordt bij opnieuw opslaan volledig overschreven", "[integration][persistence]")
{
    const JsonSaveStrategy strategy;
    const TempFile file("engine_save_strategy_overwrite" + strategy.fileExtension());
    const GameData small{.version = 1, .root = DataNode(1)};

    writeFile(file.getPath(), strategy.serialize(makeSampleData())); // lang
    writeFile(file.getPath(), strategy.serialize(small));                  // kort

    const auto result = strategy.deserialize(readFile(file.getPath()));

    REQUIRE(result.has_value()); // geen restanten van de eerste, langere inhoud
    CHECK(*result == small);
}
} // namespace engine::subsystems