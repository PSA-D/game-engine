//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IRENDERER_H
#define PROJECTSYSTEMARCHITECTURE_IRENDERER_H

#pragma once

#include "platform/DrawParams.h"
#include "platform/ITexture.h"
#include "platform/ImageData.h"
#include "platform/NativeHandles.h"
#include "utils/Color.h"
#include "utils/Rect.h"
#include "utils/Size.h"

#include <memory>

namespace engine::platform
{
// Tekent beelden op het scherm.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IPlatformFactory::createRenderer(). De aanroeper bezit hem.
// - Moet vernietigd worden vóór het window waarmee hij gemaakt is, en ná alle
//   textures die hij heeft aangemaakt.
// - Textures mogen alleen met deze renderer getekend worden.
class IRenderer
{
public:
    IRenderer() = default;
    virtual ~IRenderer() = default;
    IRenderer(const IRenderer&) = delete;
    auto operator=(const IRenderer&) -> IRenderer& = delete;
    IRenderer(IRenderer&&) = delete;
    auto operator=(IRenderer&&) -> IRenderer& = delete;

    virtual void clear(const utils::Color& color) = 0;
    virtual void drawTexture(const ITexture& texture, const DrawParams& params) = 0;
    virtual void drawRect(const utils::Rect& rect, const utils::Color& color, bool filled) = 0;
    virtual void present() = 0;

    // De aanroeper bezit de texture. Hij moet vernietigd worden vóór deze renderer.
    [[nodiscard]] virtual std::unique_ptr<ITexture> createTexture(const ImageData& data) = 0;

    [[nodiscard]] virtual utils::Size getOutputSize() const = 0; // in pixels

    // Alleen voor debug-UI-integraties. De pointers zijn niet-bezittend en alleen geldig
    // zolang deze renderer en zijn window bestaan.
    [[nodiscard]] virtual NativeHandles getNativeHandles() const = 0;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IRENDERER_H
