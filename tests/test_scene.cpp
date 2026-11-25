#include "GameObject.h"
#include "Scene.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Scene construction and identity", "[scene]")
{
    engine::Scene scene(42, "MainScene");

    REQUIRE(scene.getId() == 42);
    REQUIRE(scene.getName() == "MainScene");
    REQUIRE(scene.isActive() == false);
}

TEST_CASE("Scene initialisation and shutdown", "[scene]")
{
    engine::Scene scene(7, "TestScene");

    REQUIRE(scene.isActive() == false);
    scene.init();
    REQUIRE(scene.isActive() == true);
    scene.shutdown();
    REQUIRE(scene.isActive() == false);
}

TEST_CASE("Scene create and find game objects", "[scene]")
{
    engine::Scene scene(1, "Scene");

    engine::GameObject& first = scene.createGameObject("Player");
    engine::GameObject& second = scene.createGameObject("Enemy");

    REQUIRE(scene.findGameObject(first.getId()) == &first);
    REQUIRE(scene.findGameObject(second.getId()) == &second);
    REQUIRE(scene.find("Player") == &first);
    REQUIRE(scene.find("Enemy") == &second);
    REQUIRE(scene.findGameObject(9999) == nullptr);
    REQUIRE(scene.find("Missing") == nullptr);
}

TEST_CASE("Scene remove game object", "[scene]")
{
    engine::Scene scene(1, "Scene");

    engine::GameObject& object = scene.createGameObject("Removable");
    REQUIRE(scene.findGameObject(object.getId()) != nullptr);

    REQUIRE(scene.removeGameObject(object.getId()) == true);
    REQUIRE(scene.findGameObject(object.getId()) == nullptr);
    REQUIRE(scene.find("Removable") == nullptr);
    REQUIRE(scene.removeGameObject(object.getId()) == false);
}
