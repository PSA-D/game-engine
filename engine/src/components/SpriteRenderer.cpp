#include "components/SpriteRenderer.h"

namespace engine
{
SpriteRenderer::SpriteRenderer(GameObject& ownerObject,
                               platform::ITexture& spriteTexture,
                               DrawParams drawParams) noexcept
    : Component(ownerObject)
    , texture(spriteTexture)
    , params(drawParams)
{
}

platform::ITexture& SpriteRenderer::getTexture() const noexcept
{
    return texture;
}

const DrawParams& SpriteRenderer::getDrawParams() const noexcept
{
    return params;
}

void SpriteRenderer::setDrawParams(const DrawParams& newParams) noexcept
{
    params = newParams;
}
} // namespace engine
