#pragma once

#include "platform/Events.h"

namespace engine
{

// Event listener voor de muis
class IMouseListener
{
public:
    virtual ~IMouseListener() = default;
    virtual void onMouseButtonDown(const platform::MouseButtonEvent& button) = 0;
    virtual void onMouseButtonUp(const platform::MouseButtonEvent& button) = 0;
    virtual void onMouseMove(const platform::MouseMoveEvent& event) = 0;
    virtual void onMouseWheel(const platform::MouseWheelEvent& event) = 0;
};
} // namespace engine
