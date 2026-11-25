#include "Region.h"

#include "GameObject.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

namespace engine
{
namespace
{
auto depth(const GameObject* gameObject) -> std::size_t
{
    std::size_t result = 0;
    while (gameObject->getParent() != nullptr)
    {
        gameObject = gameObject->getParent();
        ++result;
    }
    return result;
}
} // namespace

Region::~Region() = default;

auto Region::createGameObject(std::string name) -> GameObject&
{
    auto gameObject = std::unique_ptr<GameObject>(new GameObject(*this, std::move(name)));
    GameObject& result = *gameObject;
    gameObjects.push_back(std::move(gameObject));
    return result;
}

auto Region::requestDestroy(GameObject& gameObject) -> void
{
    pendingDestruction.push_back(gameObject.getId());
}

auto Region::detach(std::uint64_t id) -> GameObject*
{
    const auto found = std::ranges::find_if(
        gameObjects, [id](const std::unique_ptr<GameObject>& candidate) { return candidate->getId() == id; });
    if (found == gameObjects.end())
    {
        return nullptr;
    }
    GameObject* result = found->release();
    gameObjects.erase(found);
    return result;
}

auto Region::adopt(std::unique_ptr<GameObject> gameObject) -> void
{
    gameObjects.push_back(std::move(gameObject));
}

auto Region::processPendingDestruction() -> void
{
    if (pendingDestruction.empty())
    {
        return;
    }

    std::vector<GameObject*> toDestroy;
    toDestroy.reserve(pendingDestruction.size());
    for (const auto id : pendingDestruction)
    {
        const auto found = std::ranges::find_if(
            gameObjects, [id](const std::unique_ptr<GameObject>& candidate) { return candidate->getId() == id; });
        if (found != gameObjects.end())
        {
            toDestroy.push_back(found->get());
        }
    }
    pendingDestruction.clear();

    // Destroy deepest objects first so children are removed before their parents.
    std::ranges::sort(toDestroy, [](GameObject* a, GameObject* b) { return depth(a) > depth(b); });

    for (GameObject* gameObject : toDestroy)
    {
        const auto found = std::ranges::find_if(gameObjects,
                                                [gameObject](const std::unique_ptr<GameObject>& candidate)
                                                { return candidate.get() == gameObject; });
        if (found != gameObjects.end())
        {
            gameObjects.erase(found);
        }
    }
}

auto Region::getGameObjects() noexcept -> std::vector<GameObject*>
{
    std::vector<GameObject*> result;
    result.reserve(gameObjects.size());
    for (const auto& gameObject : gameObjects)
    {
        result.push_back(gameObject.get());
    }
    return result;
}

auto Region::getGameObjects() const noexcept -> std::vector<const GameObject*>
{
    std::vector<const GameObject*> result;
    result.reserve(gameObjects.size());
    for (const auto& gameObject : gameObjects)
    {
        result.push_back(gameObject.get());
    }
    return result;
}
} // namespace engine
