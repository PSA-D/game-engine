#pragma once

#include "platform/IRenderer.h"

#include <memory>

namespace engine::backends
{
    class SDL3Window;

    class SDL3Renderer final : public platform::IRenderer
    {
    public:
        explicit SDL3Renderer(SDL3Window& window);
        ~SDL3Renderer() override;

        SDL3Renderer(const SDL3Renderer&) = delete;
        SDL3Renderer& operator=(const SDL3Renderer&) = delete;
        SDL3Renderer(SDL3Renderer&&) = delete;
        SDL3Renderer& operator=(SDL3Renderer&&) = delete;

        void clear(const Color& color) override;
        void drawTexture(const platform::ITexture& texture, const DrawParams& params) override;
        void present() override;
        std::unique_ptr<platform::ITexture> createTexture(const ImageData& data) override;
        [[nodiscard]] Vector2 getOutputSize() const override;
        [[nodiscard]] platform::NativeHandles getNativeHandles() const noexcept override;

    private:
        void* window{nullptr};
        void* renderer{nullptr};
    };
} // namespace engine::backends
