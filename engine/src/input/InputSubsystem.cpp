#include "InputSubsystem.h"

namespace
{
    template <typename Down, typename Pressed, typename Released>
    void applyButton(std::size_t i, bool isPressed, bool isRepeat, Down& down, Pressed& pressed, Released& released)
    {
        if (i >= down.size())
            return;

        if (isPressed)
        {
            if (!isRepeat && !down.at(i))
                pressed.at(i) = true;
            down.at(i) = true;
        }
        else
        {
            if (down.at(i))
                released.at(i) = true;
            down.at(i) = false;
        }
    }
} // namespace

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

        // Als er valide events in zitten, blijft het doorgaan.
        while (const auto e = source_.pollEvent())
        {
            processEvent(*e);
            notifyListeners(*e);
        }

        inputHandler_.handleInput(state_);
    }

    void InputSubsystem::notifyListeners(const platform::PlatformEvent& e)
    {
        if (const auto* keyEvent = std::get_if<platform::KeyEvent>(&e))
        {
            keyListeners_.dispatch(
                [&](IKeyListener& l)
                {
                    if (keyEvent->pressed)
                        l.onKeyDown(*keyEvent);
                    else
                        l.onKeyUp(*keyEvent);
                });
        }
        else if (const auto* mouseEvent = std::get_if<platform::MouseButtonEvent>(&e))
        {
            mouseListeners_.dispatch(
                [&](IMouseListener& l)
                {
                    if (mouseEvent->pressed)
                        l.onMouseButtonDown(*mouseEvent);
                    else
                        l.onMouseButtonUp(*mouseEvent);
                });
        }
        else if (const auto* mouseMoveEvent = std::get_if<platform::MouseMoveEvent>(&e))
        {
            mouseListeners_.dispatch([&](IMouseListener& l) { l.onMouseMove(*mouseMoveEvent); });
        }
        else if (const auto* mouseWheelEvent = std::get_if<platform::MouseWheelEvent>(&e))
        {
            mouseListeners_.dispatch([&](IMouseListener& l) { l.onMouseWheel(*mouseWheelEvent); });
        }
    }

    void InputSubsystem::addKeyListener(IKeyListener* listener)
    {
        keyListeners_.add(listener);
    }
    void InputSubsystem::removeKeyListener(IKeyListener* listener)
    {
        keyListeners_.remove(listener);
    }
    void InputSubsystem::addMouseListener(IMouseListener* listener)
    {
        mouseListeners_.add(listener);
    }
    void InputSubsystem::removeMouseListener(IMouseListener* listener)
    {
        mouseListeners_.remove(listener);
    }

    bool InputSubsystem::getKey(KeyCode key) const
    {
        const auto index = static_cast<std::size_t>(key);
        if (index >= state_.keysDown.size())
            return false;

        return state_.keysDown.at(index);
    }
    bool InputSubsystem::getKeyDown(KeyCode key) const
    {
        const auto index = static_cast<std::size_t>(key);
        if (index >= state_.keysPressed.size())
            return false;

        return state_.keysPressed.at(index);
    }
    bool InputSubsystem::getKeyUp(KeyCode key) const
    {
        const auto index = static_cast<std::size_t>(key);
        if (index >= state_.keysReleased.size())
            return false;

        return state_.keysReleased.at(index);
    }

    bool InputSubsystem::getMouseButton(MouseButton button) const
    {
        const auto index = static_cast<std::size_t>(button);
        if (index >= state_.mouseButtonsDown.size())
            return false;

        return state_.mouseButtonsDown.at(index);
    }
    bool InputSubsystem::getMouseButtonDown(MouseButton button) const
    {
        const auto index = static_cast<std::size_t>(button);
        if (index >= state_.mouseButtonsPressed.size())
            return false;

        return state_.mouseButtonsPressed.at(index);
    }
    bool InputSubsystem::getMouseButtonUp(MouseButton button) const
    {
        const auto index = static_cast<std::size_t>(button);
        if (index >= state_.mouseButtonsReleased.size())
            return false;

        return state_.mouseButtonsReleased.at(index);
    }

    Vector2 InputSubsystem::getMousePosition() const
    {
        return state_.mousePosition;
    }
    Vector2 InputSubsystem::getMouseDelta() const
    {
        return state_.mouseDelta;
    }
    Vector2 InputSubsystem::getMouseWheel() const
    {
        return state_.mouseWheel;
    }

    InputHandler& InputSubsystem::getHandler()
    {
        return inputHandler_;
    }
    bool InputSubsystem::isQuitRequested() const
    {
        return quitRequested_;
    }
    std::vector<platform::WindowEvent> InputSubsystem::takeWindowEvents()
    {
        std::vector<platform::WindowEvent> events;
        events.swap(windowEvents_);
        return events;
    }

    // Release alle events die zijn blijven hangen
    // als de focus weg is.
    void InputSubsystem::releaseAllHeld()
    {
        for (std::size_t i = 0; i < state_.keysDown.size(); ++i)
        {
            if (state_.keysDown.at(i))
            {
                state_.keysDown.at(i) = false;
                state_.keysReleased.at(i) = true;
            }
        }
        for (std::size_t i = 0; i < state_.mouseButtonsDown.size(); ++i)
        {
            if (state_.mouseButtonsDown.at(i))
            {
                state_.mouseButtonsDown.at(i) = false;
                state_.mouseButtonsReleased.at(i) = true;
            }
        }
    }

    void InputSubsystem::processEvent(const platform::PlatformEvent& event)
    {
        if (const auto* key = std::get_if<platform::KeyEvent>(&event))
        {
            applyButton(static_cast<std::size_t>(key->key),
                        key->pressed,
                        key->repeat,
                        state_.keysDown,
                        state_.keysPressed,
                        state_.keysReleased);
        }
        else if (const auto* btn = std::get_if<platform::MouseButtonEvent>(&event))
        {
            applyButton(static_cast<std::size_t>(btn->button),
                        btn->pressed,
                        false,
                        state_.mouseButtonsDown,
                        state_.mouseButtonsPressed,
                        state_.mouseButtonsReleased);
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
        else if (std::get_if<platform::QuitEvent>(&event) != nullptr)
        {
            quitRequested_ = true;
        }
        else if (const auto* windowEvent = std::get_if<platform::WindowEvent>(&event))
        {
            if (windowEvent->kind == platform::WindowEventKind::FocusLost)
                releaseAllHeld();
            windowEvents_.push_back(*windowEvent);
        }
    }
} // namespace engine::subsystems
