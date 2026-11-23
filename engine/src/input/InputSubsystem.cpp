#include "InputSubsystem.h"

namespace engine::subsystems
{
void InputSubsystem::update(float dt)
{
    float a = dt;
    a += 1;

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

    platform::KeyEvent ev{.key = KeyCode::ArrowLeft, .mods = {}, .pressed = true, .repeat = false};
    keyListeners_.dispatch(
        [&](IKeyListener& l)
        {
            l.onKeyUp(ev);
            l.onKeyDown(ev);
        });
}
} // namespace engine::subsystems
