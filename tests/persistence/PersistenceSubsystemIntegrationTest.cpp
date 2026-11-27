// Laag: INTEGRATIE. PersistenceSubsystem met een echte JsonSaveStrategy en echte bestanden in
// een tijdelijke map. De regels voor namen en foutteksten staan in PersistenceSubsystemTest.cpp.

#include "LevelTestData.h"
#include "PersistenceTestData.h"
#include "core/ComponentRegistry.h"
#include "runtime/Level.h"
#include "subsystems/persistence/JsonSaveStrategy.h"
#include "subsystems/persistence/PersistenceSubsystem.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace engine::subsystems
{
    namespace
    {
        // Een subsysteem op een eigen tijdelijke map.
        struct Setup
        {
            TestPaths paths;
            ComponentRegistry registry;
            PersistenceSubsystem subsystem;

            Setup()
                : subsystem(std::make_unique<JsonSaveStrategy>(), paths, registry)
            {
                registerTestComponents(registry);
            }

            [[nodiscard]] auto slotFile(const std::string& slot) const -> std::filesystem::path
            {
                return paths.getUserDataPath() / "saves" / (slot + ".json");
            }
            [[nodiscard]] auto levelFile(const std::filesystem::path& root, const std::string& name) const
                -> std::filesystem::path
            {
                return root / "levels" / (name + ".json");
            }
        };

        auto makeSave(std::int64_t score) -> GameData
        {
            auto root = DataNode::makeObject();
            root.set("score", DataNode(score));
            root.set("player", DataNode("Hero"));
            auto items = DataNode::makeArray();
            items.pushToArray(DataNode("sword"));
            items.pushToArray(DataNode("potion"));
            root.set("items", std::move(items));

            GameData data;
            data.version = PersistenceSubsystem::SAVE_GAME_VERSION;
            data.root = std::move(root);
            return data;
        }

        // Een level in JSON met een componenttype dat niet bestaat.
        constexpr const char* UNKNOWN_COMPONENT_LEVEL =
            R"({"version": 1, "root": {"name": "X", "regions": [{"name": "R", "gameObjects": [
        {"id": 1, "name": "O", "components": [{"type": "BestaatNiet", "data": {}}]}]}]}})";
    } // namespace

    // ---- Savegames ----

    TEST_CASE("Een savegame overleeft opslaan en laden", "[integration][persistence]")
    {
        Setup setup;
        const auto original = makeSave(120);

        REQUIRE(setup.subsystem.saveGame("slot1", original));
        CHECK(setup.subsystem.getLastError() == PersistenceError::None);
        CHECK(std::filesystem::exists(setup.slotFile("slot1")));

        const auto loaded = setup.subsystem.loadGame("slot1");

        REQUIRE(loaded.has_value());
        CHECK(*loaded == original);
        CHECK(setup.subsystem.getLastError() == PersistenceError::None);
    }

    TEST_CASE("Opnieuw opslaan overschrijft en laat geen tijdelijk bestand achter", "[integration][persistence]")
    {
        Setup setup;
        REQUIRE(setup.subsystem.saveGame("slot1", makeSave(1)));
        REQUIRE(setup.subsystem.saveGame("slot1", makeSave(2)));

        const auto loaded = setup.subsystem.loadGame("slot1");

        REQUIRE(loaded.has_value());
        CHECK(loaded->root.get("score")->tryInt() == 2);
        std::size_t files = 0;
        for (const auto& entry : std::filesystem::directory_iterator(setup.slotFile("slot1").parent_path()))
        {
            CHECK(entry.path().extension() == ".json"); // geen .tmp-restanten
            ++files;
        }
        CHECK(files == 1);
    }

    TEST_CASE("hasSave, listSaves en deleteSave", "[integration][persistence]")
    {
        Setup setup;
        CHECK(setup.subsystem.listSaves().empty()); // de map bestaat nog niet
        CHECK_FALSE(setup.subsystem.hasSave("alpha"));

        REQUIRE(setup.subsystem.saveGame("beta", makeSave(1)));
        REQUIRE(setup.subsystem.saveGame("alpha", makeSave(2)));
        REQUIRE(setup.subsystem.saveGame("Save 3", makeSave(3)));
        // Bestanden die geen save zijn, worden genegeerd.
        writeFileWithFolders(setup.paths.getUserDataPath() / "saves" / "readme.txt", "hallo");
        writeFileWithFolders(setup.paths.getUserDataPath() / "saves" / "x.json.tmp", "rest");

        CHECK(setup.subsystem.hasSave("alpha"));
        CHECK(setup.subsystem.listSaves() == std::vector<std::string>{"Save 3", "alpha", "beta"});

        CHECK(setup.subsystem.deleteSave("alpha"));
        CHECK_FALSE(setup.subsystem.hasSave("alpha"));
        CHECK(setup.subsystem.listSaves() == std::vector<std::string>{"Save 3", "beta"});

        CHECK_FALSE(setup.subsystem.deleteSave("alpha")); // bestaat niet meer
        CHECK(setup.subsystem.getLastError() == PersistenceError::NotFound);
    }

    TEST_CASE("Een ontbrekend bestand geeft NotFound", "[integration][persistence]")
    {
        Setup setup;

        CHECK_FALSE(setup.subsystem.loadGame("bestaat-niet").has_value());
        CHECK(setup.subsystem.getLastError() == PersistenceError::NotFound);

        CHECK(setup.subsystem.loadLevel("bestaat-niet") == nullptr);
        CHECK(setup.subsystem.getLastError() == PersistenceError::NotFound);
    }

    TEST_CASE("Een beschadigd savebestand geeft InvalidData", "[integration][persistence]")
    {
        Setup setup;
        const auto content = GENERATE(as<std::string>{},
                                      "dit is geen json",
                                      "",
                                      R"({"version": 1, "root": {"score": )", // afgebroken
                                      "[]",
                                      R"({"root": null})", // geen versie
                                      R"({"version": 0, "root": null})");
        writeFileWithFolders(setup.slotFile("kapot"), content);

        CHECK_FALSE(setup.subsystem.loadGame("kapot").has_value());
        CHECK(setup.subsystem.getLastError() == PersistenceError::InvalidData);

        // Na een fout werkt alles gewoon weer: de subsystem is niet blijven hangen.
        REQUIRE(setup.subsystem.saveGame("kapot", makeSave(5)));
        CHECK(setup.subsystem.loadGame("kapot").has_value());
        CHECK(setup.subsystem.getLastError() == PersistenceError::None);
    }

    TEST_CASE("Een savebestand van een nieuwere versie geeft UnsupportedVersion", "[integration][persistence]")
    {
        Setup setup;
        writeFileWithFolders(setup.slotFile("toekomst"), R"({"version": 999, "root": null})");

        CHECK_FALSE(setup.subsystem.loadGame("toekomst").has_value());
        CHECK(setup.subsystem.getLastError() == PersistenceError::UnsupportedVersion);
    }

    TEST_CASE("Ongeldige namen schrijven en lezen niets", "[integration][persistence]")
    {
        Setup setup;
        const auto name = GENERATE(as<std::string>{}, "../evil", "a/b", "", "con", "x.json");
        const Level level("Level");

        CHECK_FALSE(setup.subsystem.saveGame(name, makeSave(1)));
        CHECK(setup.subsystem.getLastError() == PersistenceError::InvalidName);
        CHECK_FALSE(setup.subsystem.loadGame(name).has_value());
        CHECK_FALSE(setup.subsystem.deleteSave(name));
        CHECK_FALSE(setup.subsystem.saveLevel(name, level));
        CHECK(setup.subsystem.loadLevel(name) == nullptr);
        CHECK_FALSE(setup.subsystem.hasSave(name));

        CHECK_FALSE(std::filesystem::exists(setup.paths.getRoot())); // er is niets aangemaakt
    }

    TEST_CASE("Een schrijffout wordt netjes gemeld", "[integration][persistence]")
    {
        Setup setup;
        // De gebruikersmap is een bestand, dus er kan geen map "saves" in komen.
        std::filesystem::create_directories(setup.paths.getRoot());
        writeFile(setup.paths.getUserDataPath(), "ik ben een bestand");

        CHECK_FALSE(setup.subsystem.saveGame("slot1", makeSave(1)));
        CHECK(setup.subsystem.getLastError() == PersistenceError::IoError);
        CHECK_FALSE(setup.subsystem.hasSave("slot1"));
    }

    TEST_CASE("De ingestelde ISaveStrategy bepaalt de bestandsvorm", "[integration][persistence]")
    {
        const TestPaths paths;
        ComponentRegistry registry;
        PersistenceSubsystem subsystem(std::make_unique<FakeSaveStrategy>(), paths, registry);
        GameData data;
        data.version = 1;
        data.root = DataNode("hallo");

        REQUIRE(subsystem.saveGame("slot1", data));

        const auto saves = paths.getUserDataPath() / "saves";
        REQUIRE(std::filesystem::exists(saves / "slot1.fake"));  // extensie van de strategie
        CHECK(readFile(saves / "slot1.fake") == "FAKE:1:hallo"); // inhoud van de strategie
        CHECK_FALSE(std::filesystem::exists(saves / "slot1.json"));
        const auto loaded = subsystem.loadGame("slot1");
        REQUIRE(loaded.has_value());
        CHECK(loaded->root.tryString() == "hallo");
        CHECK(subsystem.listSaves() == std::vector<std::string>{"slot1"});
    }

    // ---- Levels ----

    TEST_CASE("Een level overleeft opslaan en laden", "[integration][persistence]")
    {
        Setup setup;
        const auto original = buildSampleLevel();

        REQUIRE(setup.subsystem.saveLevel("level1", *original));
        CHECK(std::filesystem::exists(setup.levelFile(setup.paths.getUserDataPath(), "level1")));
        const auto loaded = setup.subsystem.loadLevel("level1");

        REQUIRE(loaded != nullptr);
        CHECK(snapshot(*loaded) == snapshot(*original));
    }

    TEST_CASE("loadLevel valt terug op de meegeleverde levels", "[integration][persistence]")
    {
        Setup setup;
        const auto shipped = buildSampleLevel();
        REQUIRE(setup.subsystem.saveLevel("meegeleverd", *shipped));
        const auto userFile = setup.levelFile(setup.paths.getUserDataPath(), "meegeleverd");
        const auto assetFile = setup.levelFile(setup.paths.getAssetRoot(), "meegeleverd");
        std::filesystem::create_directories(assetFile.parent_path());
        std::filesystem::rename(userFile, assetFile); // nu staat hij alleen in de assetmap

        const auto fromAssets = setup.subsystem.loadLevel("meegeleverd");

        REQUIRE(fromAssets != nullptr);
        CHECK(snapshot(*fromAssets) == snapshot(*shipped));

        // Een eigen versie in de gebruikersmap gaat voor.
        const Level own("Eigen versie");
        REQUIRE(setup.subsystem.saveLevel("meegeleverd", own));
        const auto fromUser = setup.subsystem.loadLevel("meegeleverd");
        REQUIRE(fromUser != nullptr);
        CHECK(fromUser->getName() == "Eigen versie");
    }

    TEST_CASE("Een ongeldig levelbestand geeft InvalidData", "[integration][persistence]")
    {
        Setup setup;
        const auto content = GENERATE(as<std::string>{},
                                      "dit is geen json",
                                      "",
                                      std::string(UNKNOWN_COMPONENT_LEVEL),
                                      R"({"version": 999, "root": {"name": "X", "regions": []}})");
        writeFileWithFolders(setup.levelFile(setup.paths.getUserDataPath(), "kapot"), content);

        CHECK(setup.subsystem.loadLevel("kapot") == nullptr);
        CHECK(setup.subsystem.getLastError() == PersistenceError::InvalidData);
    }
} // namespace engine::subsystems
