//
// Created by Jordy on 16/11/2026.
//

#include "platform/PlatformEvent.h"
#include "platform/WindowConfig.h"

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <variant>
#include <vector>

namespace engine::platform
{
namespace
{
template <class... Ts> struct Overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

auto describe(const PlatformEvent& event) -> std::string
{
    return std::visit(Overloaded{[](const KeyEvent&) -> std::string { return "key"; },
                                 [](const MouseButtonEvent&) -> std::string { return "mouseButton"; },
                                 [](const MouseMoveEvent&) -> std::string { return "mouseMove"; },
                                 [](const MouseWheelEvent&) -> std::string { return "mouseWheel"; },
                                 [](const WindowEvent&) -> std::string { return "window"; },
                                 [](const QuitEvent&) -> std::string { return "quit"; }},
                      event);
}
} // namespace

TEST_CASE("WindowConfig defaults describe a usable window", "[platform]")
{
    const WindowConfig config{};

    CHECK_FALSE(config.title.empty());
    CHECK(config.size.width > 0);
    CHECK(config.size.height > 0);
    CHECK_FALSE(config.fullscreen);
    CHECK(config.resizable);
}

TEST_CASE("WindowConfig fields can be overridden", "[platform]")
{
    const WindowConfig config{.title = "Test", .size = {640, 480}, .fullscreen = true};

    CHECK(config.title == "Test");
    CHECK(config.size.width == 640);
    CHECK(config.size.height == 480);
    CHECK(config.fullscreen);
    CHECK(config.resizable); // untouched default
}

TEST_CASE("PlatformEvent holds and returns each event type", "[platform]")
{
    SECTION("KeyEvent")
    {
        KeyEvent keyEvent{};
        keyEvent.pressed = true;
        keyEvent.mods.ctrl = true;
        const PlatformEvent event = keyEvent;

        const auto* result = std::get_if<KeyEvent>(&event);
        REQUIRE(result != nullptr);
        CHECK(result->pressed);
        CHECK_FALSE(result->repeat);
        CHECK(result->mods.ctrl);
        CHECK_FALSE(result->mods.shift);
    }

    SECTION("MouseButtonEvent")
    {
        MouseButtonEvent buttonEvent{};
        buttonEvent.pressed = true;
        buttonEvent.x = 10.5F;
        buttonEvent.y = 20.25F;
        const PlatformEvent event = buttonEvent;

        const auto* result = std::get_if<MouseButtonEvent>(&event);
        REQUIRE(result != nullptr);
        CHECK(result->pressed);
        CHECK(result->x == 10.5F);
        CHECK(result->y == 20.25F);
    }

    SECTION("MouseMoveEvent")
    {
        const PlatformEvent event = MouseMoveEvent{.x = 4.0F, .y = 8.0F, .dx = 1.0F, .dy = -1.0F};

        const auto* result = std::get_if<MouseMoveEvent>(&event);
        REQUIRE(result != nullptr);
        CHECK(result->dx == 1.0F);
        CHECK(result->dy == -1.0F);
    }

    SECTION("MouseWheelEvent")
    {
        const PlatformEvent event = MouseWheelEvent{.x = 0.0F, .y = 3.0F};

        const auto* result = std::get_if<MouseWheelEvent>(&event);
        REQUIRE(result != nullptr);
        CHECK(result->y == 3.0F);
    }

    SECTION("WindowEvent")
    {
        const PlatformEvent event =
            WindowEvent{.kind = WindowEventKind::Resized, .size = {800, 600}};

        const auto* result = std::get_if<WindowEvent>(&event);
        REQUIRE(result != nullptr);
        CHECK(result->kind == WindowEventKind::Resized);
        CHECK(result->size.width == 800);
        CHECK(result->size.height == 600);
    }

    SECTION("QuitEvent")
    {
        const PlatformEvent event = QuitEvent{};

        CHECK(std::holds_alternative<QuitEvent>(event));
        CHECK(std::get_if<KeyEvent>(&event) == nullptr);
    }
}

TEST_CASE("std::visit dispatches to the handler of the held type", "[platform]")
{
    const std::vector<PlatformEvent> events{KeyEvent{},        MouseButtonEvent{},
                                            MouseMoveEvent{},  MouseWheelEvent{},
                                            WindowEvent{},     QuitEvent{}};
    const std::vector<std::string> expected{"key",        "mouseButton", "mouseMove",
                                            "mouseWheel", "window",      "quit"};

    std::vector<std::string> actual;
    for (const auto& event : events)
        actual.push_back(describe(event));

    CHECK(actual == expected);
}
} // namespace engine::platform