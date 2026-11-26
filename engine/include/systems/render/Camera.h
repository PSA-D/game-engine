#pragma once

#include "Component.h"
#include "utils/Rect.h"

namespace engine
{
    class Camera final : public Component
    {
    public:
        explicit Camera(GameObject& owner) noexcept
            : Component(owner)
        {
        }

        float zoom{1.0F};
        Rect viewport{{0.0F, 0.0F}, {1.0F, 1.0F}};
        int priority{0};
    };
} // namespace engine
