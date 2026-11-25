#pragma once

#include "utils/Size.h"

#include <string>

namespace engine::platform
{
    // Het venster waarin de game getoond wordt.
    // Eigenaarschap en levensduur:
    // - Wordt aangemaakt door IPlatformFactory::createWindow(). De aanroeper bezit het.
    // - Moet vernietigd worden vóór de factory die het gemaakt heeft,
    //   en ná alles wat ermee gemaakt is (renderer, input source).
    class IWindow
    {
    public:
        IWindow() = default;
        virtual ~IWindow() = default;

        IWindow(const IWindow&) = delete;
        auto operator=(const IWindow&) -> IWindow& = delete;
        IWindow(IWindow&&) = delete;
        auto operator=(IWindow&&) -> IWindow& = delete;

        [[nodiscard]] virtual std::string getTitle() const = 0;
        [[nodiscard]] virtual Size getSize() const = 0;
        [[nodiscard]] virtual Size getPixelSize() const = 0;

        virtual void setTitle(const std::string& title) = 0;
        virtual void setFullscreen(bool enabled) = 0;
        virtual void setSize(Size size) = 0;
    };
} // namespace engine::platform
