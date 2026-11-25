#include "LoadingScene.h"
#include "SceneManager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("LoadingScene is constructible and activatable as a Scene", "[loading_scene]")
{
    engine::LoadingScene loading(1, "LoadingScreen");

    REQUIRE(loading.getId() == 1);
    REQUIRE(loading.getName() == "LoadingScreen");
    REQUIRE(loading.isActive() == false);

    loading.init();
    REQUIRE(loading.isActive() == true);

    loading.shutdown();
    REQUIRE(loading.isActive() == false);
}

TEST_CASE("LoadingScene can be created and activated through a SceneManager", "[loading_scene]")
{
    engine::EventBus eventBus;
    engine::SceneManager sceneManager(eventBus);

    // A LoadingScene is a Scene and can be driven like any other scene.
    engine::LoadingScene loading(2, "Loading");
    loading.init();
    REQUIRE(loading.isActive() == true);

    loading.shutdown();
    REQUIRE(loading.isActive() == false);

    REQUIRE(sceneManager.getActiveScene() == nullptr);
}
