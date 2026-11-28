#include "facades/InputFacade.h"

#include "subsystems/input/InputSubsystem.h"

#include <cassert>

namespace engine
{
    subsystems::InputSubsystem* InputFacade::subsystem = nullptr;

    void InputFacade::init(subsystems::InputSubsystem* input)
    {
        // Assert is eigenlijk een throw, maar dan beterl
        assert(input != nullptr && "InputFacade::init called with nullptr");
        assert(subsystem == nullptr && "InputFacade is already initialized");
        subsystem = input;
    }
    void InputFacade::shutdown()
    {
        subsystem = nullptr;
    }

    bool InputFacade::getKey(KeyCode key)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getKey(key);
    }
    bool InputFacade::getKeyDown(KeyCode key)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getKeyDown(key);
    }
    bool InputFacade::getKeyUp(KeyCode key)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getKeyUp(key);
    }

    bool InputFacade::getMouseButton(MouseButton button)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMouseButton(button);
    }
    bool InputFacade::getMouseButtonDown(MouseButton button)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMouseButtonDown(button);
    }
    bool InputFacade::getMouseButtonUp(MouseButton button)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMouseButtonUp(button);
    }

    Vector2 InputFacade::getMousePosition()
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMousePosition();
    }
    Vector2 InputFacade::getMouseDelta()
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMouseDelta();
    }
    Vector2 InputFacade::getMouseWheel()
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        return subsystem->getMouseWheel();
    }

    void InputFacade::addKeyListener(IKeyListener* listener)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem->addKeyListener(listener);
    }
    void InputFacade::removeKeyListener(IKeyListener* listener)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem->removeKeyListener(listener);
    }
    void InputFacade::addMouseListener(IMouseListener* listener)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem->addMouseListener(listener);
    }
    void InputFacade::removeMouseListener(IMouseListener* listener)
    {
        assert(subsystem != nullptr && "InputFacade used before init() or after shutdown()");
        subsystem->removeMouseListener(listener);
    }
} // namespace engine
