#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
    struct WindowConfig
    {
        static constexpr Size defaultSize{1280, 720};

        std::string title{"Game"};
        Size size{defaultSize};
        bool fullscreen{false};
        bool resizable{true};
        bool vsync{true};
    };
} // namespace engine::platform
