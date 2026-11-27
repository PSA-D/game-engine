#include "platform/headless/HeadlessWindow.h"

#include <utility>

namespace engine::platform
{
    HeadlessWindow::HeadlessWindow(WindowConfig config)
        : config(std::move(config))
    {
    }

    std::string HeadlessWindow::getTitle() const
    {
        return config.title;
    }

    Size HeadlessWindow::getSize() const
    {
        return config.size;
    }

    Size HeadlessWindow::getPixelSize() const
    {
        return config.size;
    }

    void HeadlessWindow::setTitle(const std::string& title)
    {
        config.title = title;
    }

    void HeadlessWindow::setFullscreen(bool enabled)
    {
        config.fullscreen = enabled;
    }

    void HeadlessWindow::setSize(Size size)
    {
        config.size = size;
    }

    const WindowConfig& HeadlessWindow::getConfig() const
    {
        return config;
    }
} // namespace engine::platform
