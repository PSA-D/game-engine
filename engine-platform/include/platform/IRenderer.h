#pragma once

#include "platform/DrawParams.h"
#include "platform/ITexture.h"
#include "platform/ImageData.h"
#include "platform/NativeHandles.h"
#include "utils/Color.h"
#include "utils/Vector2.h"

#include <memory>

namespace engine::platform
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual void clear(const Color& color) = 0;
        virtual void drawTexture(const ITexture& texture, const DrawParams& params) = 0;
        virtual void present() = 0;
        virtual std::unique_ptr<ITexture> createTexture(const ImageData& data) = 0;
        [[nodiscard]] virtual Vector2 getOutputSize() const = 0;
        [[nodiscard]] virtual NativeHandles getNativeHandles() const noexcept = 0;
    };
} // namespace engine::platform
