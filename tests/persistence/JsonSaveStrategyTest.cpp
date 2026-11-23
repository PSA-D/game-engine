// Laag: UNIT. Alles in het geheugen, geen bestanden, snel en geïsoleerd.
#include "persistence/JsonSaveStrategy.h"

#include "PersistenceTestData.h"
#include "persistence/ISaveStrategy.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <string>

namespace engine::subsystem::PersistenceSubsystem
{
namespace
{
using runtime::DataNode;

// Serialiseert en leest meteen terug.
auto roundTrip(const ISaveStrategy& strategy, const GameData& data) -> std::optional<GameData>
{
    return strategy.deserialize(strategy.serialize(data));
}
} // namespace

TEST_CASE("JsonSaveStrategy is bruikbaar via het ISaveStrategy-interface", "[unit][persistence]")
{
    const std::unique_ptr<ISaveStrategy> strategy = std::make_unique<JsonSaveStrategy>();

    CHECK(strategy->fileExtension() == ".json");
    CHECK_FALSE(strategy->serialize(GameData{}).empty());
}

TEST_CASE("Een round trip geeft dezelfde data terug", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;

    SECTION("Een genest voorbeeld met alle datatypen")
    {
        const GameData original = test::makeSampleData();

        const auto result = roundTrip(strategy, original);

        REQUIRE(result.has_value());
        CHECK(*result == original);
        CHECK(result->version == 3);
    }

    SECTION("Een lege root (null)")
    {
        const GameData original{};

        const auto result = roundTrip(strategy, original);

        REQUIRE(result.has_value());
        CHECK(result->root.isNull());
        CHECK(*result == original);
    }

    SECTION("Een lege lijst en een leeg object blijven wat ze zijn")
    {
        auto root = DataNode::makeObject();
        root.asObject().emplace("list", DataNode::makeArray());
        root.asObject().emplace("object", DataNode::makeObject());
        const GameData original{.version = 1, .root = std::move(root)};

        const auto result = roundTrip(strategy, original);

        REQUIRE(result.has_value());
        CHECK(result->root.asObject().at("list").isArray());
        CHECK(result->root.asObject().at("object").isObject());
        CHECK(*result == original);
    }
}

TEST_CASE("Getallen behouden hun type", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;

    SECTION("Een geheel getal blijft een geheel getal")
    {
        const auto result = roundTrip(strategy, GameData{.version = 1, .root = DataNode(42)});

        REQUIRE(result.has_value());
        REQUIRE(result->root.isInt());
        CHECK(result->root.asInt() == 42);
    }

    SECTION("Een kommagetal zonder decimalen blijft een kommagetal")
    {
        const auto result = roundTrip(strategy, GameData{.version = 1, .root = DataNode(3.0)});

        REQUIRE(result.has_value());
        REQUIRE(result->root.isDouble());
        CHECK(result->root.asDouble() == 3.0);
    }

    SECTION("De uiterste waarden van een int64 blijven exact")
    {
        constexpr auto maxValue = std::numeric_limits<std::int64_t>::max();
        constexpr auto minValue = std::numeric_limits<std::int64_t>::min();

        const auto maxResult = roundTrip(strategy, GameData{.version = 1, .root = DataNode(maxValue)});
        const auto minResult = roundTrip(strategy, GameData{.version = 1, .root = DataNode(minValue)});

        REQUIRE(maxResult.has_value());
        REQUIRE(minResult.has_value());
        CHECK(maxResult->root.asInt() == maxValue);
        CHECK(minResult->root.asInt() == minValue);
    }

    SECTION("Een negatief geheel getal")
    {
        const auto result = roundTrip(strategy, GameData{.version = 1, .root = DataNode(-7)});

        REQUIRE(result.has_value());
        CHECK(result->root.asInt() == -7);
    }
}

TEST_CASE("Tekst met bijzondere tekens overleeft een round trip", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;
    const std::string text = "Quote \" backslash \\ newline \n tab \t en é ü 日本語";

    const auto result = roundTrip(strategy, GameData{.version = 1, .root = DataNode(text)});

    REQUIRE(result.has_value());
    REQUIRE(result->root.isString());
    CHECK(result->root.asString() == text);
}

TEST_CASE("serialize gooit geen exception bij ongeldige UTF-8", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;
    const std::string invalid = "abc\xFF\xFE";

    std::string text;
    REQUIRE_NOTHROW(text = strategy.serialize(GameData{.version = 1, .root = DataNode(invalid)}));

    CHECK(strategy.deserialize(text).has_value());
}

TEST_CASE("Een niet-eindig kommagetal wordt null", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;

    const auto result =
        roundTrip(strategy, GameData{.version = 1, .root = DataNode(std::numeric_limits<double>::quiet_NaN())});

    REQUIRE(result.has_value());
    CHECK(result->root.isNull());
}

TEST_CASE("deserialize geeft nullopt bij ongeldige tekst", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;

    SECTION("Lege tekst")
    {
        CHECK_FALSE(strategy.deserialize("").has_value());
    }

    SECTION("Geen JSON")
    {
        CHECK_FALSE(strategy.deserialize("dit is geen json").has_value());
    }

    SECTION("Afgebroken JSON, zoals bij een beschadigd bestand")
    {
        CHECK_FALSE(strategy.deserialize(R"({"version": 1, "root": {"a": )").has_value());
    }

    SECTION("JSON die geen object is")
    {
        CHECK_FALSE(strategy.deserialize("[1, 2, 3]").has_value());
        CHECK_FALSE(strategy.deserialize("42").has_value());
    }

    SECTION("Versie ontbreekt")
    {
        CHECK_FALSE(strategy.deserialize(R"({"root": null})").has_value());
    }

    SECTION("Root ontbreekt")
    {
        CHECK_FALSE(strategy.deserialize(R"({"version": 1})").has_value());
    }

    SECTION("Versie heeft het verkeerde type")
    {
        CHECK_FALSE(strategy.deserialize(R"({"version": "1", "root": null})").has_value());
        CHECK_FALSE(strategy.deserialize(R"({"version": 1.5, "root": null})").has_value());
        CHECK_FALSE(strategy.deserialize(R"({"version": -1, "root": null})").has_value());
        CHECK_FALSE(strategy.deserialize(R"({"version": null, "root": null})").has_value());
    }

    SECTION("Versie is te groot voor een uint32")
    {
        CHECK_FALSE(strategy.deserialize(R"({"version": 4294967296, "root": null})").has_value());
    }
}

TEST_CASE("Onbekende extra velden in het bestand worden genegeerd", "[unit][persistence]")
{
    const JsonSaveStrategy strategy;

    const auto result = strategy.deserialize(R"({"version": 2, "root": {"a": 1}, "extra": "genegeerd"})");

    REQUIRE(result.has_value());
    CHECK(result->version == 2);
    CHECK(result->root.asObject().at("a").asInt() == 1);
}
} // namespace engine::subsystem::PersistenceSubsystem
