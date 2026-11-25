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

        auto e = source_.pollEvent();

        if (e.has_value())
        {
            if (const auto* event = std::get_if<platform::KeyEvent>(&*e))
            {
                keyListeners_.dispatch(
                    [&](IKeyListener& l)
                    {
                        l.onKeyUp(*event);
                        l.onKeyDown(*event);
                    });
            }
            if (const auto* event = std::get_if<platform::MouseButtonEvent>(&*e))
            {
                mouseListeners_.dispatch(
                    [&](IMouseListener& l)
                    {
                        l.onMouseButtonDown(*event);
                        l.onMouseButtonUp(*event);
                    });
            }
            if (const auto* event = std::get_if<platform::MouseMoveEvent>(&*e))
            {
                mouseListeners_.dispatch([&](IMouseListener& l) { l.onMouseMove(*event); });
            }
            if (const auto* event = std::get_if<platform::MouseWheelEvent>(&*e))
            {
                mouseListeners_.dispatch([&](IMouseListener& l) { l.onMouseWheel(*event); });
            }
        }

        while (const auto event = source_.pollEvent())
        {
            if (event.has_value())
                processEvent(*event);
        }

        inputHandler_.handleInput(state_);
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
