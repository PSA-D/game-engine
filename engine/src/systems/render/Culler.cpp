#include "systems/render/Culler.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace engine
{
    bool Culler::isVisible(const RenderItem& item, const CameraView& view, const Size& outputSize) const noexcept
    {
        if (item.texture == nullptr || outputSize.width <= 0 || outputSize.height <= 0 || view.zoom <= 0.0F ||
            view.viewport.size.x <= 0.0F || view.viewport.size.y <= 0.0F || item.size.x < 0.0F || item.size.y < 0.0F)
        {
            return false;
        }

        const float visibleWidth = static_cast<float>(outputSize.width) * view.viewport.size.x / view.zoom;
        const float visibleHeight = static_cast<float>(outputSize.height) * view.viewport.size.y / view.zoom;
        const float halfWidth = visibleWidth * 0.5F;
        const float halfHeight = visibleHeight * 0.5F;
        const float cameraCosine = std::cos(view.rotation);
        const float cameraSine = std::sin(view.rotation);
        const float itemCosine = std::cos(item.rotation);
        const float itemSine = std::sin(item.rotation);

        const std::array<Vector2, 4> corners{{
            {-item.pivot.x * item.size.x, -item.pivot.y * item.size.y},
            {(1.0F - item.pivot.x) * item.size.x, -item.pivot.y * item.size.y},
            {-item.pivot.x * item.size.x, (1.0F - item.pivot.y) * item.size.y},
            {(1.0F - item.pivot.x) * item.size.x, (1.0F - item.pivot.y) * item.size.y},
        }};

        float left = INFINITY;
        float right = -INFINITY;
        float top = INFINITY;
        float bottom = -INFINITY;
        for (const Vector2& corner : corners)
        {
            const float worldX = item.position.x + (itemCosine * corner.x) - (itemSine * corner.y);
            const float worldY = item.position.y + (itemSine * corner.x) + (itemCosine * corner.y);
            const float relativeX = worldX - view.position.x;
            const float relativeY = worldY - view.position.y;
            const float cameraX = (cameraCosine * relativeX) + (cameraSine * relativeY);
            const float cameraY = (-cameraSine * relativeX) + (cameraCosine * relativeY);
            left = std::min(left, cameraX);
            right = std::max(right, cameraX);
            top = std::min(top, cameraY);
            bottom = std::max(bottom, cameraY);
        }

        return right >= -halfWidth && left <= halfWidth && bottom >= -halfHeight && top <= halfHeight;
    }
} // namespace engine
