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
    // temp class
    class InputSubsystem
    {
    public:
        InputSubsystem(platform::IInputSource& source)
            : source_(source)
        {
        }

        void update();
        void processEvent(const platform::PlatformEvent& event);

        void addKeyListener(IKeyListener* listener)
        {
            keyListeners_.add(listener);
        }
        void removeKeyListener(IKeyListener* listener)
        {
            keyListeners_.remove(listener);
        }
        void addMouseListener(IMouseListener* listener)
        {
            mouseListeners_.add(listener);
        }
        void removeMouseListener(IMouseListener* listener)
        {
            mouseListeners_.remove(listener);
        }

        InputHandler& getHandler()
        {
            return inputHandler_;
        }

        bool getKey(KeyCode key)
        {
            const auto index = static_cast<std::size_t>(key);
            if (index >= state_.keysDown.size())
                return false;

            return state_.keysDown.at(index);
        };
        bool getKeyDown(KeyCode key)
        {
            const auto index = static_cast<std::size_t>(key);
            if (index >= state_.keysPressed.size())
                return false;

            return state_.keysPressed.at(index);
        };
        bool getKeyUp(KeyCode key)
        {
            const auto index = static_cast<std::size_t>(key);
            if (index >= state_.keysReleased.size())
                return false;

            return state_.keysReleased.at(index);
        };
        bool getMouseButton(MouseButton button)
        {
            const auto index = static_cast<std::size_t>(button);
            if (index >= state_.mouseButtonsDown.size())
                return false;

            return state_.mouseButtonsDown.at(index);
        };

    private:
        template <class T>
        static bool contains(const std::vector<T*>& vec, T* item)
        {
            return std::find(vec.begin(), vec.end(), item) != vec.end();
        }

        void beginFrame();
        void dispatchToListeners(const platform::PlatformEvent& event);

        platform::IInputSource& source_;
        InputState state_;
        InputHandler inputHandler_;

        bool quitRequested_{false};
        std::vector<platform::WindowEvent> windowEvents_;

        ListenersList<IKeyListener> keyListeners_;
        ListenersList<IMouseListener> mouseListeners_;
    };
} // namespace engine::subsystems
