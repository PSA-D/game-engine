#pragma once

// TIJDELIJK: wordt bij de merge vervangen door Scene.h uit feat/scene-management (Dirk).
// Zelfde pad, namespace en API als daar, maar alleen wat de GameLoop nodig heeft.

namespace engine
{
    class Scene
    {
    public:
        void update() {}
        void processPendingDestruction() {}
    };
} // namespace engine
