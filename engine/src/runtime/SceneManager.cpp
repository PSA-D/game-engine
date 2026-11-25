#include "SceneManager.h"

#include "GameObject.h"

#include <algorithm>
#include <utility>

namespace engine
{
SceneManager::SceneManager(EventBus& eventBus)
    : events_(eventBus)
{
}

SceneManager::~SceneManager() = default;

auto SceneManager::createScene(std::string_view name) -> Scene&
{
    auto scene = std::make_unique<Scene>(nextSceneId_++, std::string{name});
    Scene* result = scene.get();
    scenes_.push_back(std::move(scene));
    return *result;
}

auto SceneManager::activateScene(Scene& scene) -> bool
{
    const auto found = std::ranges::find_if(
        scenes_, [&scene](const std::unique_ptr<Scene>& candidate) { return candidate.get() == &scene; });
    if (found == scenes_.end())
    {
        return false;
    }
    if (activeScene_ == &scene)
    {
        return true;
    }
    if (activeScene_ != nullptr)
    {
        activeScene_->shutdown();
        events_.publish(SceneDeactivated{activeScene_});
    }
    activeScene_ = &scene;
    scene.init();
    events_.publish(SceneActivated{activeScene_});
    return true;
}

auto SceneManager::unloadScene(Scene& scene) -> bool
{
    const auto found = std::ranges::find_if(
        scenes_, [&scene](const std::unique_ptr<Scene>& candidate) { return candidate.get() == &scene; });
    if (found == scenes_.end())
    {
        return false;
    }
    if (activeScene_ == &scene)
    {
        scene.shutdown();
        events_.publish(SceneDeactivated{activeScene_});
        activeScene_ = nullptr;
    }
    scenes_.erase(found);
    return true;
}

auto SceneManager::getActiveScene() noexcept -> Scene*
{
    return activeScene_;
}

auto SceneManager::getActiveScene() const noexcept -> const Scene*
{
    return activeScene_;
}

auto SceneManager::findGameObject(std::uint64_t id) -> GameObject*
{
    if (activeScene_ != nullptr)
    {
        if (auto* found = activeScene_->findGameObject(id))
        {
            return found;
        }
    }
    for (const auto& scene : scenes_)
    {
        if (scene.get() == activeScene_)
        {
            continue;
        }
        if (auto* found = scene->findGameObject(id))
        {
            return found;
        }
    }
    return nullptr;
}

auto SceneManager::processPendingDestruction() -> void
{
    for (const auto& scene : scenes_)
    {
        scene->processPendingDestruction();
    }
}
} // namespace engine
