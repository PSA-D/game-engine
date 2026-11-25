#include "facades/InputFacade.h"

namespace engine
{
    subsystems::InputSubsystem* InputFacade::subsystem_ = nullptr;

    void InputFacade::init(subsystems::InputSubsystem* subsystem)
    {
        subsystem_ = subsystem;
    }
    void InputFacade::shutdown()
    {
        subsystem_ = nullptr;
    }

    bool InputFacade::getKey(KeyCode key)
    {
        return subsystem_->getKey(key);
    }
    bool InputFacade::getKeyDown(KeyCode key)
    {
        return subsystem_->getKeyDown(key);
    }
    bool InputFacade::getKeyUp(KeyCode key)
    {
        return subsystem_->getKeyUp(key);
    }

    bool InputFacade::getMouseButton(MouseButton button)
    {
        return subsystem_->getMouseButton(button);
    }
    bool InputFacade::getMouseButtonDown(MouseButton button)
    {
        return subsystem_->getMouseButtonDown(button);
    }
    bool InputFacade::getMouseButtonUp(MouseButton button)
    {
        return subsystem_->getMouseButtonUp(button);
    }

    Vector2 InputFacade::getMousePosition()
    {
        return subsystem_->getMousePosition();
    }
    Vector2 InputFacade::getMouseDelta()
    {
        return subsystem_->getMouseDelta();
    }
    Vector2 InputFacade::getMouseWheel()
    {
        return subsystem_->getMouseWheel();
    }

    void InputFacade::addKeyListener(IKeyListener* listener)
    {
        subsystem_->addKeyListener(listener);
    }
    void InputFacade::removeKeyListener(IKeyListener* listener)
    {
        subsystem_->removeKeyListener(listener);
    }
    void InputFacade::addMouseListener(IMouseListener* listener)
    {
        subsystem_->addMouseListener(listener);
    }
    void InputFacade::removeMouseListener(IMouseListener* listener)
    {
        subsystem_->removeMouseListener(listener);
    }
} // namespace engine
