// Laag: UNIT. Alles in het geheugen. De serializer wordt getest met echte runtime-klassen
// (Level, Region, GameObject zijn gewone gegevensbezitters) en een test-registry.

#include "subsystems/persistence/LevelSerializer.h"

#include "LevelTestData.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <utility>

namespace engine::subsystems
{
    namespace
    {
        using engine::DataNode;

        // Handige toegang tot onderdelen van de GameData, om ze in tests aan te passen.
        auto regionNodes(GameData& data) -> DataNode::Array&
        {
            return data.root.get("regions")->asArray();
        }
        auto objectNodes(GameData& data, std::size_t region) -> DataNode::Array&
        {
            return regionNodes(data).at(region).get("gameObjects")->asArray();
        }
        auto firstObject(GameData& data) -> DataNode&
        {
            return objectNodes(data, 0).at(0);
        }

        // Een test-opstelling: een registry met de testcomponenten en een serializer daarop.
        struct Fixture
        {
            ComponentRegistry registry;
            LevelSerializer serializer{registry};
            std::unique_ptr<Level> level;
            GameData data;

            Fixture()
            {
                registerTestComponents(registry);
                level = buildSampleLevel();
                data = serializer.toGameData(*level);
            }
        };
    } // namespace

    TEST_CASE("toGameData bewaart de structuur van een level", "[unit][persistence]")
    {
        Fixture f;

        REQUIRE(f.data.version >= 1);
        REQUIRE(f.data.root.isObject());
        CHECK(f.data.root.get("name")->tryString() == "Level 1");
        CHECK(f.data.root.get("activeRegion")->tryInt() == 1); // "Boss"

        const auto& regions = regionNodes(f.data);
        REQUIRE(regions.size() == 2);
        CHECK(regions[0].get("name")->tryString() == "Start");
        CHECK(regions[1].get("name")->tryString() == "Boss");

        const auto& objects = objectNodes(f.data, 0);
        REQUIRE(objects.size() == 3);
        CHECK(objects[0].get("name")->tryString() == "Player");
        CHECK(objects[0].get("tag")->tryString() == "Player");
        CHECK(objects[0].get("parent")->isNull());
        CHECK(objects[1].get("name")->tryString() == "Weapon");
        CHECK(objects[1].get("parent")->tryInt() == objects[0].get("id")->tryInt());
    }

    TEST_CASE("toGameData bewaart de lokale transform", "[unit][persistence]")
    {
        Fixture f;

        const auto& transform = *firstObject(f.data).get("transform");
        const auto& position = *transform.get("position")->tryArray();
        REQUIRE(position.size() == 2);
        CHECK(position[0].tryDouble() == 1.5);
        CHECK(position[1].tryDouble() == -2.0);
        CHECK(transform.get("rotation")->tryDouble() == 90.0);
        CHECK((*transform.get("scale")->tryArray())[1].tryDouble() == 3.0);
    }

    TEST_CASE("toGameData bewaart componenten met type en data", "[unit][persistence]")
    {
        Fixture f;

        const auto& components = *firstObject(f.data).get("components")->tryArray();
        REQUIRE(components.size() == 1);
        CHECK(components[0].get("type")->tryString() == "TestSprite");
        CHECK(components[0].get("enabled")->tryBool() == false);
        CHECK(components[0].get("data")->get("sprite")->tryString() == "hero.png");
    }

    TEST_CASE("toGameData slaat een niet-geregistreerd component over", "[unit][persistence]")
    {
        Fixture f;
        f.level->getRegions().front()->getGameObjects().front()->addComponent<TestUnregistered>();

        const auto data = f.serializer.toGameData(*f.level);

        const auto& components = *data.root.get("regions")
                                      ->tryArray()
                                      ->at(0)
                                      .get("gameObjects")
                                      ->tryArray()
                                      ->at(0)
                                      .get("components")
                                      ->tryArray();
        CHECK(components.size() == 1);
        // alleen de geregistreerde TestSprite
    }

    TEST_CASE("Een leeg level kan bewaard en teruggelezen worden", "[unit][persistence]")
    {
        Fixture f;
        const Level empty("Leeg");

        const auto data = f.serializer.toGameData(empty);
        const auto loaded = f.serializer.fromGameData(data);

        REQUIRE(loaded != nullptr);
        CHECK(loaded->getName() == "Leeg");
        CHECK(loaded->getRegions().empty());
        CHECK(loaded->getActiveRegion() == nullptr);
    }

    TEST_CASE("Een round trip geeft hetzelfde level terug", "[unit][persistence]")
    {
        Fixture f;

        const auto loaded = f.serializer.fromGameData(f.data);

        REQUIRE(loaded != nullptr);
        CHECK(snapshot(*loaded) == snapshot(*f.level));
    }

    TEST_CASE("Een ingelezen level gebruikt eigen ids, niet die uit het bestand", "[unit][persistence]")
    {
        Fixture f;
        // Andere ids in het bestand, ook een heel groot id (past in uint64, niet in int64).
        firstObject(f.data).set("id", DataNode(std::int64_t{-1})); // bitpatroon van UINT64_MAX
        objectNodes(f.data, 0).at(1).set("parent", DataNode(std::int64_t{-1}));

        const auto loaded = f.serializer.fromGameData(f.data);

        REQUIRE(loaded != nullptr);
        CHECK(snapshot(*loaded) == snapshot(*f.level)); // hiërarchie intact
    }

    TEST_CASE("De volgorde van objecten in het bestand maakt niet uit", "[unit][persistence]")
    {
        Fixture f;
        auto& objects = objectNodes(f.data, 0);
        std::swap(objects[0], objects[1]); // het kind (Weapon) staat nu vóór zijn ouder (Player)

        const auto loaded = f.serializer.fromGameData(f.data);

        REQUIRE(loaded != nullptr);
        const auto weapon = loaded->getRegions().front()->getGameObjects().front();
        REQUIRE(weapon->getName() == "Weapon");
        REQUIRE(weapon->getParent() != nullptr);
        CHECK(weapon->getParent()->getName() == "Player");
        CHECK(weapon->getTransform().getLocalPosition() == Vector2{0.5F, 0.0F}); // lokaal behouden
    }

    TEST_CASE("Componentdata en de enabled-vlag worden hersteld", "[unit][persistence]")
    {
        Fixture f;

        const auto loaded = f.serializer.fromGameData(f.data);

        REQUIRE(loaded != nullptr);
        const auto player = loaded->getRegions().front()->getGameObjects().front();
        const auto sprites = player->getComponents<TestSprite>();
        REQUIRE(sprites.size() == 1);
        CHECK(sprites[0]->sprite == "hero.png");
        CHECK(sprites[0]->layer == 2);
        CHECK_FALSE(sprites[0]->isEnabled());
    }

    TEST_CASE("Optionele velden mogen ontbreken en onbekende velden worden genegeerd", "[unit][persistence]")
    {
        Fixture f;
        auto& object = firstObject(f.data);
        object.asObject().erase("tag");
        object.asObject().erase("transform");
        object.asObject().erase("components");
        object.set("parent", DataNode()); // null = geen ouder
        object.set("iets_nieuws", DataNode("genegeerd"));
        f.data.root.asObject().erase("activeRegion");
        f.data.root.set("ook_nieuw", DataNode(1));

        const auto loaded = f.serializer.fromGameData(f.data);

        REQUIRE(loaded != nullptr);
        const auto player = loaded->getRegions().front()->getGameObjects().front();
        CHECK(player->getTag().empty());
        CHECK(player->getTransform().getLocalPosition() == Vector2{});
        CHECK(player->getComponents<TestSprite>().empty());
        CHECK(loaded->getActiveRegion() == nullptr);
    }

    TEST_CASE("fromGameData geeft nullptr bij ongeldige data", "[unit][persistence]")
    {
        Fixture f;
        const auto current = f.data.version;

        SECTION("Root is geen object")
        {
            f.data.root = DataNode();
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
            f.data.root = DataNode::makeArray();
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Versie 0")
        {
            f.data.version = 0;
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Versie nieuwer dan deze engine kent")
        {
            f.data.version = current + 1;
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Naam van het level ontbreekt of heeft het verkeerde type")
        {
            f.data.root.set("name", DataNode(5));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
            f.data.root.asObject().erase("name");
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Regions ontbreekt of is geen lijst")
        {
            f.data.root.set("regions", DataNode("geen lijst"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
            f.data.root.asObject().erase("regions");
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een region is geen object of heeft geen naam")
        {
            regionNodes(f.data).at(0) = DataNode(1);
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een region zonder gameObjects-lijst")
        {
            regionNodes(f.data).at(0).asObject().erase("gameObjects");
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een object mist zijn id of naam")
        {
            firstObject(f.data).asObject().erase("id");
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een object heeft een verkeerd type voor zijn naam")
        {
            firstObject(f.data).set("name", DataNode(true));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Twee objecten met hetzelfde id")
        {
            objectNodes(f.data, 0).at(1).set("id", *firstObject(f.data).get("id"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("De ouder bestaat niet")
        {
            objectNodes(f.data, 0).at(1).set("parent", DataNode(999999));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("De ouder heeft het verkeerde type")
        {
            objectNodes(f.data, 0).at(1).set("parent", DataNode("Player"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een object is zijn eigen ouder")
        {
            firstObject(f.data).set("parent", *firstObject(f.data).get("id"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Twee objecten zijn elkaars ouder (cyclus)")
        {
            firstObject(f.data).set("parent", *objectNodes(f.data, 0).at(1).get("id"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een transform met een verkeerd formaat")
        {
            auto position = DataNode::makeArray();
            position.pushToArray(DataNode(1.0));
            position.pushToArray(DataNode(2.0));
            position.pushToArray(DataNode(3.0)); // drie getallen in plaats van twee
            firstObject(f.data).get("transform")->set("position", std::move(position));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Een onbekend componenttype")
        {
            firstObject(f.data).get("components")->asArray().at(0).set("type", DataNode("BestaatNiet"));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("Ongeldige componentdata")
        {
            firstObject(f.data).get("components")->asArray().at(0).get("data")->asObject().erase("sprite");
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
        SECTION("De actieve region bestaat niet")
        {
            f.data.root.set("activeRegion", DataNode(5));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
            f.data.root.set("activeRegion", DataNode(-1));
            CHECK(f.serializer.fromGameData(f.data) == nullptr);
        }
    }
} // namespace  engine::subsystems
