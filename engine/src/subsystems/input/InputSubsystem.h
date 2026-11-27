#pragma once

#include "InputHandler.h"
#include "InputState.h"
#include "ListenersList.h"
#include "platform/IInputSource.h"
#include "runtime/IKeyListener.h"
#include "runtime/IMouseListener.h"

#include <vector>

namespace engine::subsystems
{
    class InputSubsystem
    {
    public:
        explicit InputSubsystem(platform::IInputSource& source)
            : source_(source)
        {
        }

        void update();
        void processEvent(const platform::PlatformEvent& event);

        void addKeyListener(IKeyListener* listener);
        void removeKeyListener(IKeyListener* listener);
        void addMouseListener(IMouseListener* listener);
        void removeMouseListener(IMouseListener* listener);

        bool getKey(KeyCode key) const;     // Checkt of de key wordt ingedrukt
        bool getKeyDown(KeyCode key) const; // enz.
        bool getKeyUp(KeyCode key) const;
        bool getMouseButton(MouseButton button) const;
        bool getMouseButtonDown(MouseButton button) const;
        bool getMouseButtonUp(MouseButton button) const;

        Vector2 getMousePosition() const;
        Vector2 getMouseDelta() const;
        Vector2 getMouseWheel() const;

        InputHandler& getHandler();
        bool isQuitRequested() const;
        std::vector<platform::WindowEvent> takeWindowEvents(); // Neemt alle events en leegt de lijst

    private:
        void beginFrame();
        void notifyListeners(const platform::PlatformEvent& e);
        void releaseAllHeld();

        platform::IInputSource& source_;
        InputState state_;
        InputHandler inputHandler_;

        bool quitRequested_{false};
        std::vector<platform::WindowEvent> windowEvents_;

        ListenersList<IKeyListener> keyListeners_;
        ListenersList<IMouseListener> mouseListeners_;
    };
} // namespace engine::subsystems
