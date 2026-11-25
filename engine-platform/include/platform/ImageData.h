#pragma once

#include <cstdint>
#include <vector>

namespace engine
{
struct ImageData
{
    int width{0};
    int height{0};
    std::vector<std::uint8_t> pixels;
};
} // namespace engine
