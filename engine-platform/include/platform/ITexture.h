#pragma once

namespace engine::platform
{
class ITexture
{
public:
    virtual ~ITexture() = default;

    [[nodiscard]] virtual int getWidth() const noexcept = 0;
    [[nodiscard]] virtual int getHeight() const noexcept = 0;
};
} // namespace engine::platform
