#include "subsystems/render/RenderSubsystem.h"

#include "platform/DrawParams.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace engine::subsystems
{
    namespace
    {
        auto toDrawParams(const RenderItem& item, const CameraView& view, const Vector2& outputSize) -> DrawParams
        {
            const float viewportWidth = outputSize.x * view.viewport.size.x;
            const float viewportHeight = outputSize.y * view.viewport.size.y;
            const float viewportLeft = outputSize.x * view.viewport.position.x;
            const float viewportTop = outputSize.y * view.viewport.position.y;

            const float itemCosine = std::cos(item.rotation);
            const float itemSine = std::sin(item.rotation);
            const float centerOffsetX = (0.5F - item.pivot.x) * item.size.x;
            const float centerOffsetY = (0.5F - item.pivot.y) * item.size.y;
            const float worldCenterX = item.position.x + (itemCosine * centerOffsetX) - (itemSine * centerOffsetY);
            const float worldCenterY = item.position.y + (itemSine * centerOffsetX) + (itemCosine * centerOffsetY);

            const float cameraCosine = std::cos(view.rotation);
            const float cameraSine = std::sin(view.rotation);
            const float relativeX = worldCenterX - view.position.x;
            const float relativeY = worldCenterY - view.position.y;
            const float cameraX = (cameraCosine * relativeX) + (cameraSine * relativeY);
            const float cameraY = (-cameraSine * relativeX) + (cameraCosine * relativeY);
            const float pixelWidth = item.size.x * view.zoom;
            const float pixelHeight = item.size.y * view.zoom;

            return {
                {viewportLeft + (viewportWidth * 0.5F) + (cameraX * view.zoom) - (pixelWidth * 0.5F),
                 viewportTop + (viewportHeight * 0.5F) + (cameraY * view.zoom) - (pixelHeight * 0.5F)},
                {pixelWidth, pixelHeight},
                item.rotation - view.rotation,
            };
        }
    } // namespace

    RenderSubsystem::RenderSubsystem(platform::IRenderer& rendererInterface) noexcept
        : renderer(&rendererInterface)
    {
    }

    void RenderSubsystem::beginFrame()
    {
        queue.clear();
        cameras.clear();
        if (renderer != nullptr)
        {
            renderer->clear(clearColor);
        }
    }

    void RenderSubsystem::submit(const RenderItem& item)
    {
        queue.add(item);
    }

    void RenderSubsystem::submitCamera(const CameraView& view)
    {
        cameras.push_back(view);
    }

    void RenderSubsystem::render()
    {
        if (renderer == nullptr)
        {
            if (!queue.getItems().empty() || !cameras.empty())
            {
                throw std::logic_error("RenderSubsystem requires an IRenderer to draw submitted items");
            }
            return;
        }

        const Vector2 outputSize = renderer->getOutputSize();
        if (!std::isfinite(outputSize.x) || !std::isfinite(outputSize.y) || outputSize.x < 1.0F ||
            outputSize.y < 1.0F || static_cast<double>(outputSize.x) > std::numeric_limits<int>::max() ||
            static_cast<double>(outputSize.y) > std::numeric_limits<int>::max())
        {
            throw std::runtime_error("IRenderer returned an invalid output size");
        }

        const Size outputSizeInPixels{static_cast<int>(outputSize.x), static_cast<int>(outputSize.y)};
        std::stable_sort(cameras.begin(),
                         cameras.end(),
                         [](const CameraView& left, const CameraView& right)
                         { return left.priority < right.priority; });

        for (const CameraView& camera : cameras)
        {
            RenderQueue visibleQueue;
            for (const RenderItem& item : queue.getItems())
            {
                if (culler.isVisible(item, camera, outputSizeInPixels))
                {
                    visibleQueue.add(item);
                }
            }

            visibleQueue.sort();
            for (const RenderItem& item : visibleQueue.getItems())
            {
                renderer->drawTexture(*item.texture, toDrawParams(item, camera, outputSize));
            }
        }
    }

    void RenderSubsystem::endFrame()
    {
        queue.clear();
        cameras.clear();
        if (renderer != nullptr)
        {
            renderer->present();
        }
    }

    void RenderSubsystem::setClearColor(const Color& color) noexcept
    {
        clearColor = color;
    }
} // namespace engine::subsystems
