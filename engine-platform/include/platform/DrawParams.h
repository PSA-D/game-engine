#pragma once

#include "utils/Vector2.h"

namespace engine
{
struct DrawParams
{
    Vector2 position{};
    Vector2 size{1.0F, 1.0F};
    float rotation{0.0F};
};
} // namespace engine
