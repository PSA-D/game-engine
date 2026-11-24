#pragma once

#include "utils/Vector2.h"

#include <array>

namespace engine::subsystems
{
    struct InputState
    {
        static constexpr std::size_t KEY_COUNT = static_cast<std::size_t>(KeyCode::Count);
        static constexpr std::size_t MOUSE_BUTTON_COUNT = static_cast<std::size_t>(MouseButton::Count);

        // Maakt arrays met de grootte van het aantal keys.
        // Hierdoor kan je op index zoeken of een key pressed is.
        std::array<bool, KEY_COUNT> keysDown{};
        std::array<bool, KEY_COUNT> keysPressed{};
        std::array<bool, KEY_COUNT> keysReleased{};

        // Hetzelfde voor MouseButton
        std::array<bool, MOUSE_BUTTON_COUNT> mouseButtonsDown{};
        std::array<bool, MOUSE_BUTTON_COUNT> mouseButtonsPressed{};
        std::array<bool, MOUSE_BUTTON_COUNT> mouseButtonsReleased{};

        Vector2 mousePosition{};
        Vector2 mouseDelta{};
        Vector2 mouseWheel{};
    };
} // namespace engine::subsystems
