#include "InputHandler.h"

namespace
{
    using namespace engine::subsystems;

    bool isTriggered(InputTrigger trigger, bool pressed, bool released, bool down)
    {
        switch (trigger)
        {
        case InputTrigger::Pressed:
            return pressed;
        case InputTrigger::Released:
            return released;
        case InputTrigger::Held:
            return down;
        }

        return false;
    }
} // namespace

namespace engine::subsystems
{
    void InputHandler::bindKey(KeyCode key, InputTrigger trigger, std::unique_ptr<ICommand> command)
    {
        keyBindings_.push_back({key, trigger, std::move(command)});
    }
    void InputHandler::unbindKey(KeyCode key, InputTrigger trigger)
    {
        std::erase_if(keyBindings_,
                      [key, trigger](const auto& binding) { return binding.key == key && binding.trigger == trigger; });
    }

    void InputHandler::bindMouseButton(MouseButton button, InputTrigger trigger, std::unique_ptr<ICommand> command)
    {
        mouseBindings_.push_back({button, trigger, std::move(command)});
    }
    void InputHandler::unbindMouseButton(MouseButton button, InputTrigger trigger)
    {
        std::erase_if(mouseBindings_,
                      [button, trigger](const auto& binding)
                      { return binding.button == button && binding.trigger == trigger; });
    }

    void InputHandler::clear()
    {
        keyBindings_.clear();
        mouseBindings_.clear();
    }

    void InputHandler::handleInput(const InputState& state) const
    {
        for (const auto& binding : keyBindings_)
        {
            if (!binding.command)
                continue;

            // Maakt een index op basis van de key
            const auto index = static_cast<std::size_t>(binding.key);
            if (index >= state.keysDown.size())
                continue;

            if (isTriggered(binding.trigger,
                            state.keysPressed.at(index),
                            state.keysReleased.at(index),
                            state.keysDown.at(index)))
            {
                binding.command->execute();
            }
        }

        for (const auto& binding : mouseBindings_)
        {
            if (!binding.command)
                continue;

            // Maakt een index op basis van de button
            const auto index = static_cast<std::size_t>(binding.button);
            if (index >= state.mouseButtonsDown.size())
                continue;

            if (isTriggered(binding.trigger,
                            state.mouseButtonsPressed.at(index),
                            state.mouseButtonsReleased.at(index),
                            state.mouseButtonsDown.at(index)))
            {
                binding.command->execute();
            }
        }
    }
} // namespace engine::subsystems
