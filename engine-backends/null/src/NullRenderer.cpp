#include "backends/null/NullRenderer.h"

#include <memory>

namespace engine::backends
{
    namespace
    {
        class NullTexture final : public platform::ITexture
        {
        public:
            NullTexture(int textureWidth, int textureHeight) noexcept
                : width(textureWidth)
                , height(textureHeight)
            {
            }

            int getWidth() const noexcept override
            {
                return width;
            }

            int getHeight() const noexcept override
            {
                return height;
            }

        private:
            const int width;
            const int height;
        };
    } // namespace

    void NullRenderer::clear(const Color& color)
    {
        static_cast<void>(color);
    }

    void NullRenderer::drawTexture(const platform::ITexture& texture, const DrawParams& params)
    {
        static_cast<void>(texture);
        static_cast<void>(params);
    }

    void NullRenderer::drawRect(const Rect& rect, const Color& color, bool filled)
    {
        static_cast<void>(rect);
        static_cast<void>(color);
        static_cast<void>(filled);
    }

    void NullRenderer::present() {}

    std::unique_ptr<platform::ITexture> NullRenderer::createTexture(const ImageData& data)
    {
        return std::make_unique<NullTexture>(data.width, data.height);
    }

    Vector2 NullRenderer::getOutputSize() const
    {
        return {800.0F, 600.0F};
    }

    platform::NativeHandles NullRenderer::getNativeHandles() const noexcept
    {
        return {};
    }
} // namespace engine::backends
