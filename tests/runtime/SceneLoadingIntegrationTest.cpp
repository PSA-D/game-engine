// Laag: INTEGRATIE. Scene, Level, Region en GameObject werken samen met LevelSerializer
// en JsonSaveStrategy: een level opslaan, inlezen en in een Scene gebruiken.
// De details staan in SceneTest.cpp en LevelSerializerTest.cpp.

#include "persistence/JsonSaveStrategy.h"
#include "persistence/LevelSerializer.h"
#include "src/runtime/ComponentRegistry.h"
#include "src/runtime/GameObject.h"
#include "src/runtime/Level.h"
#include "src/runtime/Region.h"
#include "src/runtime/Scene.h"
#include "LevelTestData.h"

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <utility>

namespace engine::systems
{
TEST_CASE("Een ingelezen level werkt in een Scene", "[integration][persistence]")
{
    ComponentRegistry registry;
    test::registerTestComponents(registry);
    const LevelSerializer serializer(registry);
    const JsonSaveStrategy strategy;

    // Opslaan en inlezen via JSON-tekst.
    const auto original = test::buildSampleLevel();
    const auto data = strategy.deserialize(strategy.serialize(serializer.toGameData(*original)));
    REQUIRE(data.has_value());
    auto loaded = serializer.fromGameData(*data);
    REQUIRE(loaded != nullptr);

    // In een Scene zetten. De actieve Region is "Boss", zoals opgeslagen.
    Scene scene(1, "Main");
    scene.replaceLevel(std::move(loaded));
    REQUIRE(scene.getActiveLevel() != nullptr);
    CHECK(scene.find("Boss") != nullptr);
    CHECK(scene.find("Player") == nullptr);
    // zit in de niet-actieve Region "Start"

    // Naar de andere Region wisselen: de hiërarchie is intact.
    auto* level = scene.getActiveLevel();
    level->activateRegion(*level->getRegions().front());
    auto* player = scene.find("Player");
    auto* weapon = scene.find("Weapon");
    REQUIRE(player != nullptr);
    REQUIRE(weapon != nullptr);
    CHECK(weapon->getParent() == player);

    // Het level is te gebruiken: objecten toevoegen en verwijderen.
    scene.createGameObject("Spawned");
    player->destroy();
    scene.processPendingDestruction();

    CHECK(scene.find("Player") == nullptr);
    CHECK(scene.find("Weapon") == nullptr);
    CHECK(scene.find("Camera") != nullptr);
    CHECK(scene.find("Spawned") != nullptr);

    // Het aangepaste level kan opnieuw bewaard en ingelezen worden.
    const auto again = serializer.fromGameData(serializer.toGameData(*scene.getActiveLevel()));
    REQUIRE(again != nullptr);
    CHECK(test::snapshot(*again) == test::snapshot(*scene.getActiveLevel()));
    CHECK(test::snapshot(*again).find("object Player") == std::string::npos);
}
} // namespace engine::systems