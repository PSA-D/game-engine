#include "InputSubsystem.h"

namespace engine::subsystems
{
    void InputSubsystem::beginFrame()
    {
        state_.keysPressed.fill(false);
        state_.keysReleased.fill(false);
        state_.mouseButtonsPressed.fill(false);
        state_.mouseButtonsReleased.fill(false);
        state_.mouseDelta = {};
        state_.mouseWheel = {};
    }

    void InputSubsystem::update()
    {
        beginFrame();

        // Elk event werkt eerst de toestand bij (isKeyDown e.d.) en gaat daarna naar de listeners.
        while (const auto event = source_.pollEvent())
        {
            processEvent(*event);
            dispatchToListeners(*event);
        }

        inputHandler_.handleInput(state_);
    }

    void InputSubsystem::dispatchToListeners(const platform::PlatformEvent& event)
    {
        if (const auto* keyEvent = std::get_if<platform::KeyEvent>(&event))
        {
            if (keyEvent->repeat)
                return; // onKeyDown één keer per indrukken, niet bij vasthouden

            const KeyCode key = keyEvent->key;
            const bool pressed = keyEvent->pressed;
            keyListeners_.dispatch(
                [key, pressed](IKeyListener& listener)
                {
                    if (pressed)
                        listener.onKeyDown(key);
                    else
                        listener.onKeyUp(key);
                });
        }
        else if (const auto* buttonEvent = std::get_if<platform::MouseButtonEvent>(&event))
        {
            const MouseButton button = buttonEvent->button;
            const bool pressed = buttonEvent->pressed;
            mouseListeners_.dispatch(
                [button, pressed](IMouseListener& listener)
                {
                    if (pressed)
                        listener.onMouseButtonDown(button);
                    else
                        listener.onMouseButtonUp(button);
                });
        }
        else if (const auto* moveEvent = std::get_if<platform::MouseMoveEvent>(&event))
        {
            const Vector2 position = moveEvent->position;
            mouseListeners_.dispatch([&position](IMouseListener& listener) { listener.onMouseMove(position); });
        }
        else if (const auto* wheelEvent = std::get_if<platform::MouseWheelEvent>(&event))
        {
            const float scrollDelta = wheelEvent->scroll.y;
            mouseListeners_.dispatch([scrollDelta](IMouseListener& listener) { listener.onMouseWheel(scrollDelta); });
        }
    }

    void InputSubsystem::processEvent(const platform::PlatformEvent& event)
    {
        if (const auto* keyEvent = std::get_if<platform::KeyEvent>(&event))
        {
            const auto i = static_cast<std::size_t>(keyEvent->key);
            if (i < state_.keysDown.size())
            {
                if (keyEvent->pressed)
                {
                    if (!state_.keysDown.at(i))
                        state_.keysPressed.at(i) = true;
                    state_.keysDown.at(i) = true;
                }
                else
                {
                    state_.keysDown.at(i) = false;
                    state_.keysReleased.at(i) = true;
                }
            }
        }
        else if (const auto* mouseBtn = std::get_if<platform::MouseButtonEvent>(&event))
        {
            const auto i = static_cast<std::size_t>(mouseBtn->button);
            if (i < state_.mouseButtonsDown.size())
            {
                if (mouseBtn->pressed)
                {
                    if (!state_.mouseButtonsDown.at(i))
                        state_.mouseButtonsPressed.at(i) = true;
                    state_.mouseButtonsDown.at(i) = true;
                }
                else
                {
                    state_.mouseButtonsDown.at(i) = false;
                    state_.mouseButtonsReleased.at(i) = true;
                }
            }
        }
        else if (const auto* mouseMove = std::get_if<platform::MouseMoveEvent>(&event))
        {
            state_.mousePosition = mouseMove->position;
            state_.mouseDelta.x += mouseMove->difference.x;
            state_.mouseDelta.y += mouseMove->difference.y;
        }
        else if (const auto* mouseWheel = std::get_if<platform::MouseWheelEvent>(&event))
        {
            state_.mouseWheel.x += mouseWheel->scroll.x;
            state_.mouseWheel.y += mouseWheel->scroll.y;
        }
    }
} // namespace engine::subsystems
