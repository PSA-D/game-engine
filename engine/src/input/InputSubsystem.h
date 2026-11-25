#pragma once

#include "IKeyListener.h"
#include "IMouseListener.h"
#include "InputHandler.h"
#include "InputState.h"
#include "ListenersList.h"
#include "platform/IInputSource.h"

#include <vector>

namespace engine::subsystems
{
    // Het subsyteem voor de inputs.
    // Hier wordt alles wat met input te maken heeft afgehandeld.
    class InputSubsystem
    {
    public:
        InputSubsystem(platform::IInputSource& source)
            : source_(source)
        {
        }
        ~InputSubsystem() = default;
        InputSubsystem(const InputSubsystem&) = delete;
        InputSubsystem& operator=(const InputSubsystem&) = delete;
        InputSubsystem(InputSubsystem&&) = delete;
        InputSubsystem& operator=(InputSubsystem&&) = delete;

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

        void bindKey(KeyCode key, InputTrigger trigger, std::unique_ptr<ICommand> command); // Forward naar InputHandler
        void unbindKey(KeyCode key, InputTrigger trigger);
        void bindMouseButton(MouseButton button, InputTrigger trigger, std::unique_ptr<ICommand> command);
        void unbindMouseButton(MouseButton button, InputTrigger trigger);
        void clear();

    private:
        template <class T>
        static bool contains(const std::vector<T*>& vec, T* item)
        {
            return std::find(vec.begin(), vec.end(), item) != vec.end();
        }

        void beginFrame();
        void notifyListeners(const platform::PlatformEvent& e);
        void releaseAllHeld();

        platform::IInputSource& source_;
        InputState state_;
        InputHandler handler_;

        bool quitRequested_{false};
        std::vector<platform::WindowEvent> windowEvents_;

        ListenersList<IKeyListener> keyListeners_;
        ListenersList<IMouseListener> mouseListeners_;
    };
} // namespace engine::subsystems
