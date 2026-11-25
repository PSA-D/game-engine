#pragma once

#include "core/Component.h"
#include "platform/DrawParams.h"
#include "platform/ITexture.h"

namespace engine
{
class SpriteRenderer final : public Component
{
public:
    SpriteRenderer(GameObject& owner, platform::ITexture& texture, DrawParams params = {}) noexcept;

    [[nodiscard]] platform::ITexture& getTexture() const noexcept;
    [[nodiscard]] const DrawParams& getDrawParams() const noexcept;
    void setDrawParams(const DrawParams& params) noexcept;

private:
    platform::ITexture& texture;
    DrawParams params;
};
} // namespace engine
