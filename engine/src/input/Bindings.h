#pragma once

#include "InputTrigger.h"
#include "commands/ICommand.h"
#include "utils/KeyCode.h"
#include "utils/MouseButton.h"

#include <memory>

namespace engine::subsystems
{
    struct KeyBinding
    {
        KeyCode key;
        InputTrigger trigger;
        std::unique_ptr<ICommand> command;
    };

    struct MouseBinding
    {
        MouseButton button;
        InputTrigger trigger;
        std::unique_ptr<ICommand> command;
    };
} // namespace engine::subsystems
