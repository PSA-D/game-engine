// Laag: UNIT. Alles in het geheugen. Scene wordt getest met echte Levels, Regions en GameObjects.

#include "runtime/Scene.h"

#include "runtime/Level.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <stdexcept>

namespace engine
{
    namespace
    {
        // // Een eenvoudig component om findComponents mee te testen.
        // class TestMarker final : public Component
        // {
        // public:
        //     using Component::Component;
        // };
        // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

        // Een scene met één actief level en één actieve region.
        struct ActiveScene
        {
            Scene scene{1, "Main"};
            Level* level{nullptr};
            Region* region{nullptr};

            ActiveScene()
            {
                level = &scene.createLevel("Level 1");
                region = &level->createRegion("Start");
                level->activateRegion(*region);
                scene.activateLevel(*level);
            }
        };
    } // namespace

    TEST_CASE("Een nieuwe Scene heeft een id, een naam en geen Levels", "[unit][runtime]")
    {
        const Scene scene(7, "Main");

        CHECK(scene.getId() == 7);
        CHECK(scene.getName() == "Main");
        CHECK(scene.getLevels().empty());
        CHECK(scene.getActiveLevel() == nullptr);
    }

    TEST_CASE("createLevel maakt een Level dat niet vanzelf actief is", "[unit][runtime]")
    {
        Scene scene(1, "Main");

        auto& level = scene.createLevel("Level 1");

        CHECK(level.getName() == "Level 1");
        REQUIRE(scene.getLevels().size() == 1);
        CHECK(scene.getLevels().front() == &level);
        CHECK(scene.getActiveLevel() == nullptr);
    }

    TEST_CASE("activateLevel werkt voor een eigen Level en weigert een vreemd Level", "[unit][runtime]")
    {
        Scene scene(1, "Main");
        auto& first = scene.createLevel("Een");
        auto& second = scene.createLevel("Twee");
        Scene other(2, "Andere");
        auto& foreign = other.createLevel("Vreemd");

        CHECK(scene.activateLevel(first));
        CHECK(scene.getActiveLevel() == &first);
        CHECK(scene.activateLevel(second));
        CHECK(scene.getActiveLevel() == &second);

        CHECK_FALSE(scene.activateLevel(foreign));
        CHECK(scene.getActiveLevel() == &second); // niets veranderd
    }

    // TEST_CASE("createGameObject maakt een object in de actieve Region", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //
    //     auto& player = f.scene.createGameObject("Player");
    //
    //     CHECK(&player.getRegion() == f.region);
    //     REQUIRE(f.region->getGameObjects().size() == 1);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("createGameObject volgt de actieve Region", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& other = f.level->createRegion("Boss");
    //     f.level->activateRegion(other);
    //
    //     auto& boss = f.scene.createGameObject("Boss");
    //
    //     CHECK(&boss.getRegion() == &other);
    //     CHECK(f.region->getGameObjects().empty());
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("createGameObject gooit zonder actief Level of actieve Region", "[unit][runtime]")
    // {
    //     Scene scene(1, "Main");
    //
    //     SECTION("Geen Level")
    //     {
    //         CHECK_THROWS_AS(scene.createGameObject("X"), std::logic_error);
    //     }
    //     SECTION("Een actief Level zonder actieve Region")
    //     {
    //         auto& level = scene.createLevel("Level 1");
    //         level.createRegion("Start"); // bestaat, maar is niet actief
    //         scene.activateLevel(level);
    //
    //         CHECK_THROWS_AS(scene.createGameObject("X"), std::logic_error);
    //     }
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("findGameObject zoekt op id in de actieve Region", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& player = f.scene.createGameObject("Player");
    //     auto& inactive = f.level->createRegion("Inactief");
    //     const auto hiddenId = inactive.createGameObject("Verborgen").getId();
    //
    //     CHECK(f.scene.findGameObject(player.getId()) == &player);
    //     CHECK(f.scene.findGameObject(999999999) == nullptr);
    //     CHECK(f.scene.findGameObject(hiddenId) == nullptr); // niet in de actieve Region
    //
    //     f.level->activateRegion(inactive);
    //     CHECK(f.scene.findGameObject(hiddenId) != nullptr);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("Zoeken zonder actieve inhoud geeft niets terug", "[unit][runtime]")
    // {
    //     Scene scene(1, "Main");
    //
    //     CHECK(scene.findGameObject(1) == nullptr);
    //     CHECK(scene.find("X") == nullptr);
    //     CHECK(scene.findWithTag("X").empty());
    //     CHECK(scene.findComponents<TestMarker>().empty());
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("find geeft het eerste object met die naam", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& first = f.scene.createGameObject("Enemy");
    //     f.scene.createGameObject("Enemy");
    //
    //     CHECK(f.scene.find("Enemy") == &first);
    //     CHECK(f.scene.find("Bestaat niet") == nullptr);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("findWithTag geeft alle objecten met die tag", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& first = f.scene.createGameObject("A");
    //     auto& second = f.scene.createGameObject("B");
    //     f.scene.createGameObject("C");
    //     first.setTag("Enemy");
    //     second.setTag("Enemy");
    //
    //     const auto enemies = f.scene.findWithTag("Enemy");
    //
    //     REQUIRE(enemies.size() == 2);
    //     CHECK(enemies[0] == &first);
    //     CHECK(enemies[1] == &second);
    //     CHECK(f.scene.findWithTag("Onbekend").empty());
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("findComponents verzamelt componenten van één type", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& first = f.scene.createGameObject("A");
    //     auto& second = f.scene.createGameObject("B");
    //     f.scene.createGameObject("Zonder");
    //     auto& markerOne = first.addComponent<TestMarker>();
    //     auto& markerTwo = second.addComponent<TestMarker>();
    //
    //     const auto markers = f.scene.findComponents<TestMarker>();
    //
    //     REQUIRE(markers.size() == 2);
    //     CHECK(markers[0] == &markerOne);
    //     CHECK(markers[1] == &markerTwo);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // TEST_CASE("replaceLevel vervangt het actieve Level en activeert het nieuwe", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& kept = f.scene.createLevel("Blijft");
    //     f.scene.createGameObject("Oud");
    //
    //     auto incoming = std::make_unique<Level>("Nieuw");
    //     auto* incomingPointer = incoming.get();
    //     f.scene.replaceLevel(std::move(incoming));
    //
    //     CHECK(f.scene.getActiveLevel() == incomingPointer);
    //     CHECK(f.scene.getActiveLevel()->getName() == "Nieuw");
    //     const auto levels = f.scene.getLevels();
    //     REQUIRE(levels.size() == 2); // het oude actieve Level is weg, het andere blijft
    //     CHECK(levels[0] == incomingPointer); // op dezelfde plek als het oude
    //     CHECK(levels[1] == &kept);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

    TEST_CASE("replaceLevel zonder actief Level voegt het nieuwe toe en activeert het", "[unit][runtime]")
    {
        Scene scene(1, "Main");
        scene.createLevel("Bestaand");

        auto incoming = std::make_unique<Level>("Nieuw");
        auto* incomingPointer = incoming.get();
        scene.replaceLevel(std::move(incoming));

        CHECK(scene.getLevels().size() == 2);
        CHECK(scene.getActiveLevel() == incomingPointer);
    }

    TEST_CASE("replaceLevel gooit bij nullptr en verandert niets", "[unit][runtime]")
    {
        ActiveScene f;

        CHECK_THROWS_AS(f.scene.replaceLevel(nullptr), std::invalid_argument);

        CHECK(f.scene.getActiveLevel() == f.level);
        CHECK(f.scene.getLevels().size() == 1);
    }

    // TEST_CASE("processPendingDestruction ruimt alle Regions van alle Levels op", "[unit][runtime]")
    // {
    //     ActiveScene f;
    //     auto& inactiveRegion = f.level->createRegion("Inactief");
    //     auto& otherLevel = f.scene.createLevel("Ander level");
    //     auto& otherRegion = otherLevel.createRegion("Elders");
    //
    //     f.scene.processPendingDestruction();
    //
    //     CHECK(f.region->getGameObjects().empty());
    //     CHECK(inactiveRegion.getGameObjects().empty());
    //     CHECK(otherRegion.getGameObjects().empty());
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
} // namespace engine
