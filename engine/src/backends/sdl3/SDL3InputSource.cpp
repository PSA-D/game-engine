#include "SDL3InputSource.h"

#include "SDL3KeyMap.h"

using namespace engine::platform;

namespace engine::backends
{
std::optional<PlatformEvent> SDL3InputSource::pollEvent()
{
    SDL_Event sdlEvent;

    // Blijft pollen tot het een translatable event vind of de queue leeg is
    while (SDL_PollEvent(&sdlEvent))
    {
        bool consumed = false;
        for (INativeEventListener* listener : listeners_)
        {
            if (listener != nullptr && listener->onNativeEvent(&sdlEvent))
                consumed = true;
        }

        // Continue, zodat een UI button klikken niet de speler laat springen ofzo
        if (consumed)
            continue;

        if (auto translated = translateEvent(sdlEvent))
            return translated;
    }

    return std::nullopt;
}

void SDL3InputSource::addNativeEventListener(INativeEventListener* listener)
{
    if (listener != nullptr)
        listeners_.push_back(listener);
}

void SDL3InputSource::removeNativeEventListener(INativeEventListener* listener)
{
    std::erase(listeners_, listener);
}

std::optional<PlatformEvent> SDL3InputSource::translateEvent(const SDL_Event& event) const
{
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
        return QuitEvent{};

    // Window Events
    case SDL_EVENT_WINDOW_RESIZED:
        return WindowEvent{WindowEventKind::Resized,
                           Vector2{static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)}};
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        return WindowEvent{WindowEventKind::FocusGained, window_.getSize()};
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        return WindowEvent{WindowEventKind::FocustLost, window_.getSize()};
    case SDL_EVENT_WINDOW_MINIMIZED:
        return WindowEvent{WindowEventKind::Minimized, window_.getSize()};
    case SDL_EVENT_WINDOW_MAXIMIZED:
        return WindowEvent{WindowEventKind::Maximized, window_.getSize()};
    case SDL_EVENT_WINDOW_RESTORED:
        return WindowEvent{WindowEventKind::Restored, window_.getSize()};
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        return WindowEvent{WindowEventKind::CloseRequested, window_.getSize()};

    // Mouse Events
    case SDL_EVENT_MOUSE_MOTION:
        return MouseMoveEvent{event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel};
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        MouseButton btn = SDL3KeyMap::toMouseButton(event.button.button);
        if (btn == MouseButton::Unknown)
            return std::nullopt;

        return MouseButtonEvent{btn, event.button.down, event.button.x, event.button.y};
    }
    case SDL_EVENT_MOUSE_WHEEL:
        return MouseWheelEvent{event.wheel.x, event.wheel.y};

    // Key Events
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    {
        KeyModifiers mods = SDL3KeyMap::toKeyModifiers(event.key.mod);
        KeyCode code = SDL3KeyMap::toKeyCode(event.key.scancode);
        return KeyEvent{code, mods, event.key.down, event.key.repeat};
    }

    default:
        return std::nullopt; // Unhandled SDL events
    }
}
} // namespace engine::backends
