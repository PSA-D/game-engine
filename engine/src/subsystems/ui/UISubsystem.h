#pragma once

namespace engine::subsystems
{
    // Leeg tot de UI-US. Nog geen diagram; render() wordt na de scene en na debug aangeroepen.
    class UISubsystem
    {
    public:
        void update(float /*deltaTime*/) {}
        void render() {}
    };
} // namespace engine::subsystems
