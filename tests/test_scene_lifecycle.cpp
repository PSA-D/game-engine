#include "Scene.h"
#include "SceneManager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Region processPendingDestruction removes destroyed GameObjects", "[scene_lifecycle]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject("Temp");
    object.destroy();

    REQUIRE(region.getGameObjects().size() == 1);

    region.processPendingDestruction();

    REQUIRE(region.getGameObjects().empty());
}

TEST_CASE("Region destroys children before parents", "[scene_lifecycle]")
{
    engine::Region region;
    engine::GameObject& parent = region.createGameObject("Parent");
    engine::GameObject& child = region.createGameObject("Child");
    REQUIRE(child.setParent(&parent));

    parent.destroy();
    child.destroy();

    region.processPendingDestruction();

    REQUIRE(region.getGameObjects().empty());
}

TEST_CASE("Scene processPendingDestruction removes destroyed GameObjects", "[scene_lifecycle]")
{
    engine::Scene scene(1, "Test");
    engine::GameObject& object = scene.createGameObject("Obj");
    const auto id = object.getId();
    object.destroy();

    REQUIRE(scene.findGameObject(id) != nullptr);

    scene.processPendingDestruction();

    REQUIRE(scene.findGameObject(id) == nullptr);
}

TEST_CASE("Scene update only processes active scene GameObjects", "[scene_lifecycle]")
{
    engine::Scene scene(1, "Test");
    scene.createGameObject("Obj");

    scene.update();
    scene.render();

    scene.init();
    scene.update();
    scene.render();

    scene.shutdown();
    scene.update();
    scene.render();
}

TEST_CASE("SceneManager processPendingDestruction delegates to all scenes", "[scene_lifecycle]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& scene1 = manager.createScene("Scene1");
    engine::Scene& scene2 = manager.createScene("Scene2");
    engine::GameObject& go1 = scene1.createGameObject("Obj1");
    engine::GameObject& go2 = scene2.createGameObject("Obj2");
    const auto id1 = go1.getId();
    const auto id2 = go2.getId();
    go1.destroy();
    go2.destroy();

    REQUIRE(scene1.findGameObject(id1) != nullptr);
    REQUIRE(scene2.findGameObject(id2) != nullptr);

    manager.processPendingDestruction();

    REQUIRE(scene1.findGameObject(id1) == nullptr);
    REQUIRE(scene2.findGameObject(id2) == nullptr);
}

TEST_CASE("Repeated scene switches do not leave destroyed GameObjects behind", "[scene_lifecycle]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    for (int i = 0; i < 5; ++i)
    {
        engine::Scene& scene = manager.createScene("Scene");
        engine::GameObject& object = scene.createGameObject("Obj");
        const auto id = object.getId();
        object.destroy();
        manager.activateScene(scene);
        manager.processPendingDestruction();
        REQUIRE(manager.getActiveScene() == &scene);
        REQUIRE(scene.findGameObject(id) == nullptr);
        manager.unloadScene(scene);
        REQUIRE(manager.getActiveScene() == nullptr);
    }
}

TEST_CASE("Unloading active scene leaves no active scene and cleans up GameObjects", "[scene_lifecycle]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& scene = manager.createScene("Temp");
    engine::GameObject& object = scene.createGameObject("Obj");
    const auto id = object.getId();
    manager.activateScene(scene);
    REQUIRE(manager.getActiveScene() == &scene);

    manager.unloadScene(scene);
    REQUIRE(manager.getActiveScene() == nullptr);

    manager.processPendingDestruction();
    REQUIRE(manager.findGameObject(id) == nullptr);
}
