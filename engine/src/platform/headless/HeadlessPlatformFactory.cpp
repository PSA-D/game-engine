#include "platform/headless/HeadlessPlatformFactory.h"

#include "platform/headless/HeadlessInputSource.h"
#include "platform/headless/HeadlessWindow.h"

namespace engine::platform
{
    std::unique_ptr<IWindow> HeadlessPlatformFactory::createWindow(const WindowConfig& config)
    {
        return std::make_unique<HeadlessWindow>(config);
    }

    std::unique_ptr<IInputSource> HeadlessPlatformFactory::createInputSource(IWindow& /*window*/)
    {
        return std::make_unique<HeadlessInputSource>();
    }
} // namespace engine::platform
