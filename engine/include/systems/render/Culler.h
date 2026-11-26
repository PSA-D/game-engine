#pragma once

#include "systems/render/CameraView.h"
#include "systems/render/RenderItem.h"
#include "utils/Size.h"

namespace engine
{
    class Culler
    {
    public:
        [[nodiscard]] bool isVisible(const RenderItem& item,
                                     const CameraView& view,
                                     const Size& outputSize) const noexcept;
    };
} // namespace engine
