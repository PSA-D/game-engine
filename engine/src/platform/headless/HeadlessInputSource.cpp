#include "platform/headless/HeadlessInputSource.h"

namespace engine::platform
{
    std::optional<PlatformEvent> HeadlessInputSource::pollEvent()
    {
        return std::nullopt;
    }

    void HeadlessInputSource::addNativeEventListener(INativeEventListener* /*listener*/) {}

    void HeadlessInputSource::removeNativeEventListener(INativeEventListener* /*listener*/) {}
} // namespace engine::platform
