#pragma once

#include "Bindings.h"
#include "InputState.h"
#include "InputTrigger.h"
#include "commands/ICommand.h"
#include "utils/InputCodes.h"

#include <vector>

namespace engine::subsystems
{
    class InputHandler
    {
    public:
        void bindKey(KeyCode key, InputTrigger trigger, std::unique_ptr<ICommand> command);
        void unbindKey(KeyCode key, InputTrigger trigger);
        void bindMouseButton(MouseButton button, InputTrigger trigger, std::unique_ptr<ICommand> command);
        void unbindMouseButton(MouseButton button, InputTrigger trigger);
        void clear();
        void handleInput(InputState state);

    private:
        std::vector<KeyBinding> keyBindings_;
        std::vector<MouseBinding> mouseBindings_;
    };
} // namespace engine::subsystems
