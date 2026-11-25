#pragma once

// TIJDELIJK: wordt bij de merge vervangen door SceneManager.h uit feat/scene-management (Dirk).
// Zelfde pad, namespace en API als daar, maar alleen wat de GameLoop nodig heeft.
// Let op bij de merge: Dirk's SceneManager vraagt een EventBus& in de constructor (zie EngineImpl).

#include "Scene.h"

namespace engine
{
    class SceneManager
    {
    public:
        // Nog geen scenes: de GameLoop slaat physics, scripts, audio en render(scene) dan over.
        [[nodiscard]] Scene* getActiveScene() noexcept
        {
            return nullptr;
        }

        [[nodiscard]] const Scene* getActiveScene() const noexcept
        {
            return nullptr;
        }

        void processPendingDestruction() {}
    };
} // namespace engine
