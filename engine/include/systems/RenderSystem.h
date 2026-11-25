#pragma once

#include "platform/IRenderer.h"

#include <vector>

namespace engine
{
class GameObject;

class RenderSystem
{
public:
    explicit RenderSystem(platform::IRenderer& renderer) noexcept;

    void render(const std::vector<GameObject*>& gameObjects, const Color& clearColor);

private:
    platform::IRenderer& renderer;
};
} // namespace engine
