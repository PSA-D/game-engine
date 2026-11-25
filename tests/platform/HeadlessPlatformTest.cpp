#include "platform/WindowConfig.h"
#include "platform/headless/HeadlessPlatformFactory.h"
#include "platform/headless/HeadlessWindow.h"
#include "utils/Size.h"

#include <catch2/catch_test_macros.hpp>

namespace engine::platform
{
    TEST_CASE("HeadlessWindow geeft de waarden uit zijn WindowConfig terug", "[unit][platform]")
    {
        WindowConfig config;
        config.title = "Venster";
        config.size = {320, 240};

        const HeadlessWindow window{config};

        CHECK(window.getTitle() == "Venster");
        CHECK(window.getSize() == Size{320, 240});
        CHECK(window.getPixelSize() == Size{320, 240});
    }

    TEST_CASE("HeadlessWindow onthoudt wijzigingen via de setters", "[unit][platform]")
    {
        HeadlessWindow window{WindowConfig{}};

        window.setTitle("Nieuw");
        window.setSize({1024, 768});
        window.setFullscreen(true);

        CHECK(window.getTitle() == "Nieuw");
        CHECK(window.getSize() == Size{1024, 768});
        CHECK(window.getConfig().fullscreen);
    }

    TEST_CASE("HeadlessPlatformFactory maakt een HeadlessWindow", "[unit][platform]")
    {
        HeadlessPlatformFactory factory;

        const auto window = factory.createWindow(WindowConfig{});

        REQUIRE(window != nullptr);
        CHECK(dynamic_cast<HeadlessWindow*>(window.get()) != nullptr);
    }
} // namespace engine::platform
