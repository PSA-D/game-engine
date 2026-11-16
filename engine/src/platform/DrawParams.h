//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_DRAWPARAMS_H
#define PROJECTSYSTEMARCHITECTURE_DRAWPARAMS_H

#pragma once

#include "utils/Color.h"
#include "utils/Rect.h"
#include "utils/Vector2.h"

namespace engine::platform
{
struct DrawParams
{
    utils::Rect source{};              // regio van de texture, in pixels
    utils::Rect destination{};         // doelwit van de output, in pixels
    float rotation{0.0F};              // graden
    utils::Vector2 pivot{0.5F, 0.5F};  // genormaliseerd 0..1, rotatiepunt
    utils::Color tint{255, 255, 255, 255}; // wit = geen tint
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_DRAWPARAMS_H
