//
// Created by Jordy on 16/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_IMAGEDATA_H
#define PROJECTSYSTEMARCHITECTURE_IMAGEDATA_H

#pragma once

#include "utils/Size.h"

#include <cstdint>
#include <vector>

namespace engine::platform
{
// Gecodeerde afbeelding, compact verpakt in RGBA-formaat, 4 bytes per pixel, rij voor rij.
struct ImageData
{
    utils::Size size{};
    std::vector<std::uint8_t> pixels;
};
} // namespace engine::platform

#endif // PROJECTSYSTEMARCHITECTURE_IMAGEDATA_H
