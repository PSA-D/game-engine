#pragma once

#include "platform/IRenderer.h"

namespace engine::backends
{
    class NullRenderer final : public platform::IRenderer
    {
    public:
        void clear(const Color& color) override;
        void drawTexture(const platform::ITexture& texture, const DrawParams& params) override;
        void present() override;
        std::unique_ptr<platform::ITexture> createTexture(const ImageData& data) override;
        [[nodiscard]] Vector2 getOutputSize() const override;
        [[nodiscard]] platform::NativeHandles getNativeHandles() const noexcept override;
    };
} // namespace engine::backends
