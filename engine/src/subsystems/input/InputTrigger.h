#pragma once

#include <cstdint>

namespace engine::subsystems
{
    enum class InputTrigger : uint8_t
    {
        Pressed,
        Held,
        Released,
    };
} // namespace engine::subsystems
