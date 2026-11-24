#include "InputSubsystem.h"

#include <iostream>
#include <ostream>

namespace engine::subsystems
{
    void InputSubsystem::update(float dt)
    {
        float a = dt; // Even om een unused error te voorkomen
        a += 1;
        std::cout << "a = " << a << '\n';

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
    }

    void InputSubsystem::processEvent(const platform::PlatformEvent&)
    {

    }
} // namespace engine::subsystems
