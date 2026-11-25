#pragma once

// TIJDELIJK: wordt bij de merge vervangen door InputSubsystem.h uit feat/input-subsystem (Yanick).
// Zelfde namespace en API als daar, maar alleen wat de GameLoop nodig heeft.
// Let op: Yanick's versie staat in src/input/; bij de merge verplaatsen naar src/subsystems/input/.
// Tot die merge zijn er geen events, dus het venster reageert niet op het kruisje.

namespace engine::subsystems
{
    class InputSubsystem
    {
    public:
        void update() {}

        [[nodiscard]] bool isQuitRequested() const
        {
            return false;
        }
    };
} // namespace engine::subsystems
