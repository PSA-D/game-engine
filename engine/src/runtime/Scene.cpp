#include "Scene.h"

#include "GameObject.h"

#include <algorithm>
#include <string_view>
#include <utility>

namespace engine
{
Scene::Scene(std::uint64_t sceneId, std::string sceneName)
    : id(sceneId)
    , name(std::move(sceneName))
{
}

Scene::~Scene() = default;

void Scene::init()
{
    active = true;
}

void Scene::shutdown()
{
    active = false;
}

auto Scene::isActive() const noexcept -> bool
{
    return active;
}

auto Scene::getId() const noexcept -> std::uint64_t
{
    return id;
}

auto Scene::getName() const noexcept -> const std::string&
{
    return name;
}

auto Scene::createGameObject(std::string objectName) -> GameObject&
{
    GameObject& gameObject = region.createGameObject(std::move(objectName));
    gameObjects.push_back(&gameObject);
    return gameObject;
}

auto Scene::removeGameObject(std::uint64_t gameObjectId) noexcept -> bool
{
    const auto found = std::ranges::find_if(
        gameObjects, [gameObjectId](const GameObject* candidate) { return candidate->getId() == gameObjectId; });
    if (found == gameObjects.end())
    {
        return false;
    }
    gameObjects.erase(found);
    return true;
}

auto Scene::findGameObject(std::uint64_t gameObjectId) noexcept -> GameObject*
{
    const auto found = std::ranges::find_if(
        gameObjects, [gameObjectId](const GameObject* candidate) { return candidate->getId() == gameObjectId; });
    return found == gameObjects.end() ? nullptr : *found;
}

auto Scene::findGameObject(std::uint64_t gameObjectId) const noexcept -> const GameObject*
{
    const auto found = std::ranges::find_if(
        gameObjects, [gameObjectId](const GameObject* candidate) { return candidate->getId() == gameObjectId; });
    return found == gameObjects.end() ? nullptr : *found;
}

auto Scene::find(std::string_view objectName) noexcept -> GameObject*
{
    const auto found = std::ranges::find_if(
        gameObjects, [&objectName](const GameObject* candidate) { return candidate->getName() == objectName; });
    return found == gameObjects.end() ? nullptr : *found;
}

auto Scene::find(std::string_view objectName) const noexcept -> const GameObject*
{
    const auto found = std::ranges::find_if(
        gameObjects, [&objectName](const GameObject* candidate) { return candidate->getName() == objectName; });
    return found == gameObjects.end() ? nullptr : *found;
}

void Scene::update()
{
    if (!active)
    {
        return;
    }
    for (auto* gameObject : gameObjects)
    {
        gameObject->update();
    }
}

void Scene::render()
{
    if (!active)
    {
        return;
    }
    for (auto* gameObject : gameObjects)
    {
        gameObject->render();
    }
}

void Scene::processPendingDestruction()
{
    region.processPendingDestruction();
    auto current = region.getGameObjects();
    gameObjects.assign(current.begin(), current.end());
}
} // namespace engine
