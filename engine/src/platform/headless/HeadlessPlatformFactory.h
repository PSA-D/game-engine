#pragma once

#include "platform/IPlatformFactory.h"

namespace engine::platform
{
    // Factory zonder echt platform. Maakt onderdelen die niets tonen en niets afspelen.
    class HeadlessPlatformFactory final : public IPlatformFactory
    {
    public:
        [[nodiscard]] std::unique_ptr<IWindow> createWindow(const WindowConfig& config) override;
    };
} // namespace engine::platform
