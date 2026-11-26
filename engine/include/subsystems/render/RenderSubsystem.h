#pragma once

#include "platform/IRenderer.h"
#include "systems/render/CameraView.h"
#include "systems/render/Culler.h"
#include "systems/render/RenderQueue.h"
#include "utils/Color.h"

#include <vector>

namespace engine::subsystems
{
    class RenderSubsystem
    {
    public:
        RenderSubsystem() noexcept = default;
        explicit RenderSubsystem(platform::IRenderer& renderer) noexcept;

        void beginFrame();
        void submit(const RenderItem& item);
        void submitCamera(const CameraView& view);
        void render();
        void endFrame();
        void setClearColor(const Color& color) noexcept;

    private:
        platform::IRenderer* renderer{nullptr};
        Color clearColor{};
        RenderQueue queue;
        std::vector<CameraView> cameras;
        Culler culler;
    };
} // namespace engine::subsystems
