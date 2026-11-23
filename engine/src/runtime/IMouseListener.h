#pragma once

#include "src/utils/InputCodes.h"
#include "src/utils/Vector2.h"

namespace engine
{
class IMouseListener
{
public:
    virtual ~IMouseListener() = default;
    virtual void onMouseMove(const Vector2& position) = 0;
    virtual void onMouseButtonDown(MouseButton button) = 0;
    virtual void onMouseButtonUp(MouseButton button) = 0;
    virtual void onMouseWheel(float scrollDelta) = 0;
};
} // namespace engine
