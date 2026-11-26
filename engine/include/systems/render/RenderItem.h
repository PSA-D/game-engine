#pragma once

#include "platform/ITexture.h"
#include "utils/Color.h"
#include "utils/Rect.h"
#include "utils/Vector2.h"

namespace engine
{
    struct RenderItem
    {
        const platform::ITexture* texture{nullptr};
        Rect sourceRect{};
        Vector2 position{};
        float rotation{0.0F};
        Vector2 pivot{0.5F, 0.5F};
        Vector2 size{1.0F, 1.0F};
        Color tint{255, 255, 255, 255};
        int sortingLayer{0};
        int orderInLayer{0};
    };
} // namespace engine
