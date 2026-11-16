//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_ITEXTURE_H
#define PROJECTSYSTEMARCHITECTURE_ITEXTURE_H

#pragma once

namespace engine::platform
{
// Een afbeelding die klaarstaat om getekend te worden.
// Eigenaarschap en levensduur:
// - Wordt aangemaakt door IRenderer::createTexture(). De aanroeper bezit hem (unique_ptr).
// - Mag alleen getekend worden door de renderer die hem heeft aangemaakt.
// - Moet vernietigd worden vóór die renderer.
class ITexture
{
public:
    ITexture() = default;
    virtual ~ITexture() = default;
    ITexture(const ITexture&) = delete;
    auto operator=(const ITexture&) -> ITexture& = delete;
    ITexture(ITexture&&) = delete;
    auto operator=(ITexture&&) -> ITexture& = delete;

    [[nodiscard]] virtual int getWidth() const = 0;  // in pixels
    [[nodiscard]] virtual int getHeight() const = 0; // in pixels
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_ITEXTURE_H
