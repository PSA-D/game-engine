#pragma once

namespace engine::subsystems
{
    // Leeg tot de debug-US. Signature volgens DebugSubsystemV2.
    // update() krijgt de ONgeschaalde deltaTime, zodat de FPS-meting ook bij timeScale = 0 klopt.
    class DebugSubsystem
    {
    public:
        void update(float /*unscaledDeltaTime*/) {}
        void render() {}
    };
} // namespace engine::subsystems
