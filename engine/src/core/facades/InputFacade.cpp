#include "facades/InputFacade.h"

#include <cassert>

namespace engine
{
    InputSubsystem* InputFacade::subsystem_ = nullptr;

    void InputFacade::init(InputSubsystem* subsystem)
    {
        // Assert is eigenlijk een throw, maar dan beter
        assert(subsystem != nullptr && "InputFacade::init called with nullptr");
        assert(subsystem_ == nullptr && "InputFacade is already initialized");
        subsystem_ = subsystem;
    }
    void InputFacade::shutdown()
    {
        subsystem_ = nullptr;
    }

    bool InputFacade::getKey(KeyCode key)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getKey(key);
    }
    bool InputFacade::getKeyDown(KeyCode key)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getKeyDown(key);
    }
    bool InputFacade::getKeyUp(KeyCode key)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getKeyUp(key);
    }

    bool InputFacade::getMouseButton(MouseButton button)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMouseButton(button);
    }
    bool InputFacade::getMouseButtonDown(MouseButton button)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMouseButtonDown(button);
    }
    bool InputFacade::getMouseButtonUp(MouseButton button)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMouseButtonUp(button);
    }

    Vector2 InputFacade::getMousePosition()
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMousePosition();
    }
    Vector2 InputFacade::getMouseDelta()
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMouseDelta();
    }
    Vector2 InputFacade::getMouseWheel()
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem_->getMouseWheel();
    }

    void InputFacade::addKeyListener(IKeyListener* listener)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->addKeyListener(listener);
    }
    void InputFacade::removeKeyListener(IKeyListener* listener)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->removeKeyListener(listener);
    }
    void InputFacade::addMouseListener(IMouseListener* listener)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->addMouseListener(listener);
    }
    void InputFacade::removeMouseListener(IMouseListener* listener)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->removeMouseListener(listener);
    }

    void InputFacade::bindKey(KeyCode key, InputTrigger trigger, std::unique_ptr<ICommand> command)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->bindKey(key, trigger, std::move(command));
    }
    void InputFacade::unbindKey(KeyCode key, InputTrigger trigger)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->unbindKey(key, trigger);
    }
    void InputFacade::bindMouseButton(MouseButton button, InputTrigger trigger, std::unique_ptr<ICommand> command)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->bindMouseButton(button, trigger, std::move(command));
    }
    void InputFacade::unbindMouseButton(MouseButton button, InputTrigger trigger)
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->unbindMouseButton(button, trigger);
    }
    void InputFacade::clear()
    {
        assert(subsystem_ != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem_->clear();
    }
} // namespace engine
