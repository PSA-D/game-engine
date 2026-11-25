#pragma once

namespace engine
{
    class Scene;
} // namespace engine

namespace engine::subsystems
{
    // Leeg tot de animation-US. Nog geen diagram; zelfde vorm als de andere subsystemen.
    class AnimationSubsystem
    {
    public:
        void update(float /*deltaTime*/, Scene& /*scene*/) {}
    };
} // namespace engine::subsystems
