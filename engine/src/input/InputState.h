#pragma once

#include "utils/Vector2.h"

#include <array>

namespace engine::subsystems
{
    struct InputState
    {
        static constexpr std::size_t MAX_INPUTS = 15;
        static constexpr std::size_t MAX_BUTTONS_CLICKED = 5;

        std::array<bool, MAX_INPUTS> keysDown{};
        std::array<bool, MAX_INPUTS> keysPressed{};
        std::array<bool, MAX_INPUTS> keysReleased{};

        std::array<bool, MAX_BUTTONS_CLICKED> mouseButtonsDown{};
        std::array<bool, MAX_BUTTONS_CLICKED> mouseButtonsPressed{};
        std::array<bool, MAX_BUTTONS_CLICKED> mouseButtonsReleased{};

        Vector2 mousePosition{};
        Vector2 mouseDelta{};
        Vector2 mouseWheel{};
    };
} // namespace engine::subsystems
