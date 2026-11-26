#pragma once

#include "utils/Rect.h"
#include "utils/Vector2.h"

namespace engine
{
    struct CameraView
    {
        Vector2 position{};
        float rotation{0.0F};
        float zoom{1.0F};
        Rect viewport{{0.0F, 0.0F}, {1.0F, 1.0F}};
        int priority{0};
    };
} // namespace engine
