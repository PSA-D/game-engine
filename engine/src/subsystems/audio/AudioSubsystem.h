#pragma once

namespace engine
{
    class Scene;
} // namespace engine

namespace engine::subsystems
{
    // Leeg tot de audio-US. Signature volgens AudioSubsystemV2.
    class AudioSubsystem
    {
    public:
        void update(float /*deltaTime*/, Scene& /*scene*/) {}
    };
} // namespace engine::subsystems
