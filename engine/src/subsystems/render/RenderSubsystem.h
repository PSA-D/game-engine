#pragma once

namespace engine
{
    class Scene;
} // namespace engine

namespace engine::subsystems
{
    // Leeg tot de render-US. Signature volgens RenderSubsystemV2 [F8]:
    // beginFrame (clear) → render(scene) → debug.render() → ui.render() → endFrame (present).
    class RenderSubsystem
    {
    public:
        void beginFrame() {}
        void render(Scene& /*scene*/) {}
        void endFrame() {}
    };
} // namespace engine::subsystems
