#include "systems/RenderSystem.h"

#include "components/SpriteRenderer.h"
#include "core/GameObject.h"

namespace engine
{
RenderSystem::RenderSystem(platform::IRenderer& rendererInterface) noexcept
    : renderer(rendererInterface)
{
}

void RenderSystem::render(const std::vector<GameObject*>& gameObjects, const Color& clearColor)
{
    renderer.clear(clearColor);
    for (GameObject* gameObject : gameObjects)
    {
        if (gameObject == nullptr)
        {
            continue;
        }

        SpriteRenderer* sprite = gameObject->getComponent<SpriteRenderer>();
        if (sprite != nullptr && sprite->isEnabled())
        {
            renderer.drawTexture(sprite->getTexture(), sprite->getDrawParams());
        }
    }
    renderer.present();
}
} // namespace engine
