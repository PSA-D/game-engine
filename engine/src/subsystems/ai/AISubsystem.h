#pragma once

namespace engine
{
    class Scene;
} // namespace engine

namespace engine::subsystems
{
    // Leeg tot de AI-US. Nog geen diagram; zelfde vorm als de andere subsystemen.
    class AISubsystem
    {
    public:
        void update(float /*deltaTime*/, Scene& /*scene*/) {}
    };
} // namespace engine::subsystems
