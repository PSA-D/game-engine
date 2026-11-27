#pragma once

#include "utils/MouseButton.h"
#include "utils/Vector2.h"

namespace engine
{
    // Event listener voor de muis (volgens het KD).
    class IMouseListener
    {
    public:
        virtual ~IMouseListener() = default;
        virtual void onMouseMove(const Vector2& position) = 0;
        virtual void onMouseButtonDown(MouseButton button) = 0;
        virtual void onMouseButtonUp(MouseButton button) = 0;
        virtual void onMouseWheel(float scrollDelta) = 0; // verticaal; positief = van je af
    };
} // namespace engine
