#include "core/CompositionRoot.h"

#include "EngineConfig.h"
#include "platform/IWindow.h"
#include "utils/Size.h"

#include <catch2/catch_test_macros.hpp>

namespace engine::core
{
    TEST_CASE("makeWindowConfig neemt alle vensterinstellingen over uit de EngineConfig", "[unit][core]")
    {
        EngineConfig config;
        config.appName = "Dungeon Crawler";
        config.windowSize = {800, 600};
        config.fullscreen = true;
        config.resizable = false;
        config.vsync = false;

        const platform::WindowConfig windowConfig = makeWindowConfig(config);

        CHECK(windowConfig.title == "Dungeon Crawler");
        CHECK(windowConfig.size == Size{800, 600});
        CHECK(windowConfig.fullscreen);
        CHECK_FALSE(windowConfig.resizable);
        CHECK_FALSE(windowConfig.vsync);
    }

    TEST_CASE("makeWindowConfig gebruikt de standaardwaarden van een lege EngineConfig", "[unit][core]")
    {
        const platform::WindowConfig windowConfig = makeWindowConfig(EngineConfig{});

        CHECK(windowConfig.title == "Game");
        CHECK(windowConfig.size == Size{1280, 720});
        CHECK_FALSE(windowConfig.fullscreen);
        CHECK(windowConfig.resizable);
        CHECK(windowConfig.vsync);
    }

    TEST_CASE("createPlatformFactory(Headless) maakt een venster met titel en size uit de config", "[unit][core]")
    {
        EngineConfig config;
        config.appName = "Headless";
        config.windowSize = {640, 480};

        const auto factory = createPlatformFactory(PlatformBackend::Headless);
        REQUIRE(factory != nullptr);

        const auto window = factory->createWindow(makeWindowConfig(config));
        REQUIRE(window != nullptr);

        CHECK(window->getTitle() == "Headless");
        CHECK(window->getSize() == Size{640, 480});
    }
} // namespace engine::core
