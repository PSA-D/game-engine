#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SceneManager starts with no active scene", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    REQUIRE(manager.getActiveScene() == nullptr);
}

TEST_CASE("SceneManager createScene registers and names scenes", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& first = manager.createScene("Menu");
    engine::Scene& second = manager.createScene("Gameplay");

    REQUIRE(first.getName() == "Menu");
    REQUIRE(second.getName() == "Gameplay");
    REQUIRE(first.getId() != second.getId());
    REQUIRE(manager.getActiveScene() == nullptr);
}

TEST_CASE("SceneManager activateScene sets the active scene", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& scene = manager.createScene("Menu");

    REQUIRE(manager.activateScene(scene));
    REQUIRE(manager.getActiveScene() == &scene);
    REQUIRE(scene.isActive());
}

TEST_CASE("SceneManager switching deactivates previous and activates next", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& first = manager.createScene("Menu");
    engine::Scene& second = manager.createScene("Gameplay");

    REQUIRE(manager.activateScene(first));
    REQUIRE(first.isActive());
    REQUIRE(!second.isActive());

    REQUIRE(manager.activateScene(second));
    REQUIRE(!first.isActive());
    REQUIRE(second.isActive());
    REQUIRE(manager.getActiveScene() == &second);
}

TEST_CASE("SceneManager activating an unmanaged scene fails", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene orphan(99, "Orphan");

    REQUIRE(!manager.activateScene(orphan));
    REQUIRE(manager.getActiveScene() == nullptr);
}

TEST_CASE("SceneManager unloadScene removes the scene", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& scene = manager.createScene("Temp");
    REQUIRE(manager.activateScene(scene));

    REQUIRE(manager.unloadScene(scene));
    REQUIRE(manager.getActiveScene() == nullptr);
}

TEST_CASE("SceneManager unloading non-active scene keeps active scene", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& first = manager.createScene("Menu");
    engine::Scene& second = manager.createScene("Gameplay");
    REQUIRE(manager.activateScene(first));

    REQUIRE(manager.unloadScene(second));
    REQUIRE(manager.getActiveScene() == &first);
    REQUIRE(first.isActive());
}

TEST_CASE("SceneManager findGameObject locates object in active scene", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    engine::Scene& scene = manager.createScene("Menu");
    engine::GameObject& object = scene.createGameObject("Player");
    REQUIRE(manager.activateScene(scene));

    REQUIRE(manager.findGameObject(object.getId()) == &object);
}

TEST_CASE("SceneManager findGameObject returns nullptr for unknown id", "[scene_manager]")
{
    engine::EventBus eventBus;
    engine::SceneManager manager(eventBus);

    manager.createScene("Menu");
    REQUIRE(manager.findGameObject(9999) == nullptr);
}

TEST_CASE("EventBus publish and dispatch delivers to subscriber", "[event_bus]")
{
    engine::EventBus eventBus;

    int received = 0;
    auto sub = eventBus.subscribe<int>([&received](const int& value) { received = value; });

    eventBus.publish(42);
    eventBus.dispatchPending();

    REQUIRE(received == 42);
}

TEST_CASE("EventBus dispatchPending only processes events queued at cycle start", "[event_bus]")
{
    engine::EventBus eventBus;

    int count = 0;
    auto sub = eventBus.subscribe<int>(
        [&count, &eventBus](const int&)
        {
            ++count;
            eventBus.publish(99); // published during dispatch; must wait for next cycle
        });

    eventBus.publish(1);
    eventBus.dispatchPending();

    REQUIRE(count == 1);
    eventBus.dispatchPending();
    REQUIRE(count == 2);
}

TEST_CASE("EventBus subscription lifetime ends on unsubscribe", "[event_bus]")
{
    engine::EventBus eventBus;

    int received = 0;
    {
        auto sub = eventBus.subscribe<int>([&received](const int& value) { received = value; });
        eventBus.publish(7);
        eventBus.dispatchPending();
        REQUIRE(received == 7);
    } // subscription destroyed here

    received = 0;
    eventBus.publish(8);
    eventBus.dispatchPending();
    REQUIRE(received == 0);
}
