#pragma once

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

        void update(float dt);
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

    private:
        template <class T>
        static bool contains(const std::vector<T*>& vec, T* item)
        {
            return std::find(vec.begin(), vec.end(), item) != vec.end();
        }

        platform::IInputSource& source_;
        InputState state_;

        ListenersList<IKeyListener> keyListeners_;
        ListenersList<IMouseListener> mouseListeners_;
    };
} // namespace engine::subsystems
