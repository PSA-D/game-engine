#pragma once

#include "platform/IInputSource.h"

#include <optional>

namespace engine::platform
{
    // Een input source zonder apparaat: levert nooit events.
    // Voor tests en CI, waar geen toetsenbord of muis is.
    class HeadlessInputSource final : public IInputSource
    {
    public:
        [[nodiscard]] std::optional<PlatformEvent> pollEvent() override;
        void addNativeEventListener(INativeEventListener* listener) override;
        void removeNativeEventListener(INativeEventListener* listener) override;
    };
} // namespace engine::platform
