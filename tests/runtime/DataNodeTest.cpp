// Laag: UNIT. Alles in het geheugen, geen bestanden, snel en geïsoleerd.
#include "runtime/DataNode.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <string>
#include <variant>

namespace engine::runtime
{
TEST_CASE("Een nieuwe DataNode is null", "[unit][runtime]")
{
    const DataNode node;

    CHECK(node.isNull());
    CHECK_FALSE(node.isBool());
    CHECK_FALSE(node.isInt());
    CHECK_FALSE(node.isDouble());
    CHECK_FALSE(node.isString());
    CHECK_FALSE(node.isArray());
    CHECK_FALSE(node.isObject());
}

TEST_CASE("Elk datatype geeft zijn eigen waarde terug", "[unit][runtime]")
{
    CHECK(DataNode(true).asBool());
    CHECK(DataNode(5).asInt() == 5);
    CHECK(DataNode(std::int64_t{9000000000}).asInt() == 9000000000);
    CHECK(DataNode(2.5).asDouble() == 2.5);
    CHECK(DataNode(std::string("tekst")).asString() == "tekst");
}

TEST_CASE("Een const char* wordt een tekst en geen bool", "[unit][runtime]")
{
    const DataNode node("hallo");

    CHECK(node.isString());
    CHECK_FALSE(node.isBool());
    CHECK(node.asString() == "hallo");
}

TEST_CASE("Een int wordt een geheel getal en geen kommagetal", "[unit][runtime]")
{
    const DataNode node(3);

    CHECK(node.isInt());
    CHECK_FALSE(node.isDouble());
}

TEST_CASE("Lijsten en objecten kunnen genest worden", "[unit][runtime]")
{
    auto list = DataNode::makeArray();
    list.asArray().push_back(DataNode(1));
    list.asArray().push_back(DataNode("twee"));

    auto root = DataNode::makeObject();
    root.asObject().emplace("list", list);

    REQUIRE(root.isObject());
    const auto& stored = root.asObject().at("list");
    REQUIRE(stored.isArray());
    REQUIRE(stored.asArray().size() == 2);
    CHECK(stored.asArray()[0].asInt() == 1);
    CHECK(stored.asArray()[1].asString() == "twee");
}

TEST_CASE("Een getter met het verkeerde type gooit een exception", "[unit][runtime]")
{
    const DataNode node(5);

    CHECK_THROWS_AS(node.asString(), std::bad_variant_access);
    CHECK_THROWS_AS(node.asArray(), std::bad_variant_access);
}

TEST_CASE("Gelijkheid vergelijkt de hele boom", "[unit][runtime]")
{
    auto first = DataNode::makeObject();
    first.asObject().emplace("a", DataNode(1));
    auto second = DataNode::makeObject();
    second.asObject().emplace("a", DataNode(1));

    CHECK(first == second);

    second.asObject().at("a") = DataNode(2);
    CHECK_FALSE(first == second);
    CHECK_FALSE(DataNode(1) == DataNode(1.0)); // verschillende typen zijn niet gelijk
}

TEST_CASE("Een kopie is onafhankelijk van het origineel", "[unit][runtime]")
{
    auto original = DataNode::makeArray();
    original.asArray().push_back(DataNode(1));

    DataNode copy = original;
    copy.asArray().push_back(DataNode(2));

    CHECK(original.asArray().size() == 1);
    CHECK(copy.asArray().size() == 2);
}
TEST_CASE("get geeft een kind van een object, of nullptr", "[unit][runtime]")
{
    auto object = DataNode::makeObject();
    object.set("a", DataNode(1));

    REQUIRE(object.get("a") != nullptr);
    CHECK(object.get("a")->asInt() == 1);
    CHECK(object.get("ontbreekt") == nullptr);
    CHECK(DataNode(5).get("a") == nullptr); // geen object
    CHECK(DataNode().get("a") == nullptr);
}

TEST_CASE("set vervangt een bestaande waarde", "[unit][runtime]")
{
    auto object = DataNode::makeObject();
    object.set("a", DataNode(1));
    object.set("a", DataNode("nieuw"));

    CHECK(object.get("a")->asString() == "nieuw");
    CHECK(object.asObject().size() == 1);
}

TEST_CASE("set en pushToArray gooien als het type niet klopt", "[unit][runtime]")
{
    DataNode number(5);
    auto list = DataNode::makeArray();
    auto object = DataNode::makeObject();

    CHECK_THROWS_AS(number.set("a", DataNode()), std::bad_variant_access);
    CHECK_THROWS_AS(list.set("a", DataNode()), std::bad_variant_access);
    CHECK_THROWS_AS(object.pushToArray(DataNode()), std::bad_variant_access);
}

TEST_CASE("pushToArray voegt achteraan toe", "[unit][runtime]")
{
    auto list = DataNode::makeArray();
    list.pushToArray(DataNode(1));
    list.pushToArray(DataNode("twee"));

    REQUIRE(list.asArray().size() == 2);
    CHECK(list.asArray()[1].asString() == "twee");
}

TEST_CASE("try-getters geven nullopt of nullptr bij een verkeerd type", "[unit][runtime]")
{
    CHECK(DataNode(true).tryBool() == true);
    CHECK_FALSE(DataNode(1).tryBool().has_value());
    CHECK(DataNode(7).tryInt() == 7);
    CHECK_FALSE(DataNode(7.5).tryInt().has_value()); // een kommagetal is geen geheel getal
    CHECK(DataNode("x").tryString() == "x");
    CHECK_FALSE(DataNode(1).tryString().has_value());
    CHECK(DataNode::makeArray().tryArray() != nullptr);
    CHECK(DataNode::makeArray().tryObject() == nullptr);
    CHECK(DataNode::makeObject().tryObject() != nullptr);
    CHECK(DataNode().tryArray() == nullptr);
}

TEST_CASE("tryDouble accepteert ook een geheel getal", "[unit][runtime]")
{
    CHECK(DataNode(2.5).tryDouble() == 2.5);
    CHECK(DataNode(3).tryDouble() == 3.0);
    CHECK_FALSE(DataNode("3").tryDouble().has_value());
    CHECK_FALSE(DataNode().tryDouble().has_value());
}
} // namespace engine::runtime