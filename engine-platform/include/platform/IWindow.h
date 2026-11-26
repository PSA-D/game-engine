#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        [[nodiscard]] virtual std::string getTitle() const = 0;
        [[nodiscard]] virtual Size getSize() const = 0;
        [[nodiscard]] virtual Size getPixelSize() const = 0;

        virtual void setTitle(const std::string& title) = 0;
        virtual void setFullscreen(bool enabled) = 0;
        virtual void setSize(Size size) = 0;
    };
} // namespace engine::platform
