// Laag: INTEGRATIE. Level -> GameData -> JSON-tekst -> bestand -> en weer terug naar een Level.
// Hier werken LevelSerializer, JsonSaveStrategy, DataNode en het bestandssysteem samen.
// De details van de serializer staan in LevelSerializerTest.cpp.

#include "LevelTestData.h"
#include "TempFile.h"
#include "persistence/JsonSaveStrategy.h"
#include "persistence/LevelSerializer.h"

#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <string>

namespace  engine::subsystems
{
    namespace
    {
        struct Setup
        {
            runtime::ComponentRegistry registry;
            LevelSerializer serializer{registry};
            JsonSaveStrategy strategy;

            Setup()
            {
                test::registerTestComponents(registry);
            }

            // Level -> tekst op schijf -> tekst -> Level.
            auto saveAndLoad(const runtime::Level& level, const std::string& fileName)
                -> std::unique_ptr<runtime::Level>
            {
                const test::TempFile file(fileName + strategy.fileExtension());
                test::writeFile(file.getPath(), strategy.serialize(serializer.toGameData(level)));

                const auto data = strategy.deserialize(test::readFile(file.getPath()));
                if (!data)
                    return nullptr;
                return serializer.fromGameData(*data);
            }
        };
    } // namespace

    TEST_CASE("Een level overleeft opslaan in een JSON-bestand en weer inlezen", "[integration][persistence]")
    {
        Setup setup;
        const auto original = test::buildSampleLevel();

        const auto loaded = setup.saveAndLoad(*original, "engine_level_roundtrip");

        REQUIRE(loaded != nullptr);
        CHECK(test::snapshot(*loaded) == test::snapshot(*original));
    }

    TEST_CASE("Een ingelezen level kan opnieuw gebruikt worden in de game", "[integration][persistence]")
    {
        Setup setup;
        const auto original = test::buildSampleLevel();
        const auto loaded = setup.saveAndLoad(*original, "engine_level_reuse");
        REQUIRE(loaded != nullptr);

        SECTION("Nieuwe objecten toevoegen en het geheel opnieuw bewaren")
        {
            auto& start = *loaded->getRegions().front();
            auto& extra = start.createGameObject("Extra");
            extra.addComponent<test::TestSprite>().sprite = "extra.png";

            const auto again = setup.saveAndLoad(*loaded, "engine_level_reuse_again");

            REQUIRE(again != nullptr);
            CHECK(test::snapshot(*again) == test::snapshot(*loaded));
            CHECK(again->getRegions().front()->getGameObjects().size() == 4);
        }

        SECTION("Een component aanpassen en opnieuw bewaren")
        {
            auto* player = loaded->getRegions().front()->getGameObjects().front();
            player->getComponents<test::TestSprite>().front()->layer = 9;

            const auto again = setup.saveAndLoad(*loaded, "engine_level_modify");

            REQUIRE(again != nullptr);
            const auto* againPlayer = again->getRegions().front()->getGameObjects().front();
            CHECK(againPlayer->getComponents<test::TestSprite>().front()->layer == 9);
        }

        SECTION("Opnieuw bewaren verandert niets (stabiel na meerdere keren)")
        {
            const auto second = setup.saveAndLoad(*loaded, "engine_level_stable_2");
            REQUIRE(second != nullptr);
            const auto third = setup.saveAndLoad(*second, "engine_level_stable_3");
            REQUIRE(third != nullptr);

            CHECK(test::snapshot(*third) == test::snapshot(*original));
        }
    }

    TEST_CASE("Een groot level met een diepe hiërarchie blijft correct", "[integration][persistence]")
    {
        Setup setup;
        runtime::Level level("Groot");
        auto& region = level.createRegion("Alles");
        runtime::GameObject* previous = nullptr;
        for (int index = 0; index < 300; ++index)
        {
            auto& object = region.createGameObject("Object " + std::to_string(index));
            object.getTransform().setLocalPosition({static_cast<float>(index), 0.0F});
            if (previous != nullptr)
                object.setParent(previous, false);
            // een keten van 300 objecten diep
            previous = &object;
        }
        level.activateRegion(region);

        const auto loaded = setup.saveAndLoad(level, "engine_level_large");

        REQUIRE(loaded != nullptr);
        CHECK(test::snapshot(*loaded) == test::snapshot(level));
    }

    TEST_CASE("Geldige JSON met een ongeldig level geeft nullptr en geen crash", "[integration][persistence]")
    {
        Setup setup;
        const test::TempFile file("engine_level_invalid" + setup.strategy.fileExtension());

        SECTION("Onbekend componenttype")
        {
            test::writeFile(file.getPath(), R"({"version": 1, "root": {"name": "X", "regions": [
            {"name": "R", "gameObjects": [{"id": 1, "name": "O", "components":
                [{"type": "BestaatNiet", "data": {}}]}]}]}})");
        }
        SECTION("Een versie uit de toekomst")
        {
            test::writeFile(file.getPath(), R"({"version": 999, "root": {"name": "X", "regions": []}})");
        }
        SECTION("Een afgebroken bestand")
        {
            test::writeFile(file.getPath(), R"({"version": 1, "root": {"name": "X", "regio)");
        }

        const auto data = setup.strategy.deserialize(test::readFile(file.getPath()));
        const auto level = data ? setup.serializer.fromGameData(*data) : nullptr;

        CHECK(level == nullptr);
    }
} // namespace engine::subsystems
