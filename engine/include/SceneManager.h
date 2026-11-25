#pragma once

#include "EventBus.h"
#include "Scene.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace engine
{
class GameObject;

struct SceneActivated
{
    Scene* scene;
};

struct SceneDeactivated
{
    Scene* scene;
};

class SceneManager
{
public:
    explicit SceneManager(EventBus& eventBus);
    ~SceneManager();

    SceneManager(const SceneManager&) = delete;
    auto operator=(const SceneManager&) -> SceneManager& = delete;
    SceneManager(SceneManager&&) = delete;
    auto operator=(SceneManager&&) noexcept -> SceneManager& = delete;

    Scene& createScene(std::string_view name);
    bool activateScene(Scene& scene);
    bool unloadScene(Scene& scene);

    [[nodiscard]] Scene* getActiveScene() noexcept;
    [[nodiscard]] const Scene* getActiveScene() const noexcept;

    [[nodiscard]] GameObject* findGameObject(std::uint64_t id);

private:
    EventBus& events_;
    std::vector<std::unique_ptr<Scene>> scenes_;
    Scene* activeScene_{nullptr};
    std::uint64_t nextSceneId_{1};
};
} // namespace engine
