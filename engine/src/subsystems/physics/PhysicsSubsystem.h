#pragma once

namespace engine
{
    class Scene;
} // namespace engine

namespace engine::subsystems
{
    // Leeg tot de physics-US. Signature volgens PhysicsSubsystemV2.
    // De FixedTimestep komt hierbinnen; de GameLoop geeft alleen de frametijd door.
    class PhysicsSubsystem
    {
    public:
        void update(float /*deltaTime*/, Scene& /*scene*/) {}
    };
} // namespace engine::subsystems
