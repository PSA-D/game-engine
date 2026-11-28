#include "runtime/Scene.h"

#include "runtime/GameObject.h"
#include "runtime/Level.h"
#include "runtime/Region.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace engine
{
    Scene::Scene(std::uint64_t sceneId, std::string sceneName)
        : id(sceneId)
        , name(std::move(sceneName))
    {
    }

    Scene::~Scene() = default;

    std::uint64_t Scene::getId() const
    {
        return id;
    }

    const std::string& Scene::getName() const
    {
        return name;
    }

    Level& Scene::createLevel(std::string_view levelName)
    {
        levels.push_back(std::make_unique<Level>(std::string(levelName)));
        return *levels.back();
    }

    bool Scene::activateLevel(Level& level)
    {
        const bool owned = std::any_of(
            levels.begin(), levels.end(), [&level](const auto& candidate) { return candidate.get() == &level; });
        if (!owned)
            return false;

        activeLevel = &level;
        return true;
    }

    Level* Scene::getActiveLevel()
    {
        return activeLevel;
    }

    const Level* Scene::getActiveLevel() const
    {
        return activeLevel;
    }

    std::vector<Level*> Scene::getLevels()
    {
        std::vector<Level*> result;
        result.reserve(levels.size());
        for (auto& level : levels)
            result.push_back(level.get());
        return result;
    }

    std::vector<const Level*> Scene::getLevels() const
    {
        std::vector<const Level*> result;
        result.reserve(levels.size());
        for (const auto& level : levels)
            result.push_back(level.get());
        return result;
    }

    void Scene::replaceLevel(std::unique_ptr<Level> level)
    {
        if (level == nullptr)
            throw std::invalid_argument("Scene::replaceLevel: het level is nullptr.");

        Level* incoming = level.get();

        const auto current = std::find_if(
            levels.begin(), levels.end(), [this](const auto& owned) { return owned.get() == activeLevel; });
        if (activeLevel != nullptr && current != levels.end())
            *current = std::move(level);
        // op dezelfde plek, het oude level wordt hier vernietigd
        else
            levels.push_back(std::move(level));

        activeLevel = incoming;
    }

    GameObject& Scene::createGameObject(std::string_view objectName) const
    {
        auto* region = getActiveRegion();
        if (region == nullptr)
        {
            throw std::logic_error("Scene::createGameObject: er is geen actief level met een actieve region.");
        }
        return region->createGameObject(objectName);
    }

    GameObject* Scene::findGameObject(std::uint64_t objectId) const
    {
        for (auto* gameObject : collectActiveGameObjects())
        {
            if (gameObject->getId() == objectId)
                return gameObject;
        }
        return nullptr;
    }

    GameObject* Scene::find(std::string_view objectName) const
    {
        for (auto* gameObject : collectActiveGameObjects())
        {
            if (gameObject->getName() == objectName)
                return gameObject;
        }
        return nullptr;
    }

    std::vector<GameObject*> Scene::findWithTag(std::string_view tag) const
    {
        std::vector<GameObject*> result;
        for (auto* gameObject : collectActiveGameObjects())
        {
            if (gameObject->getTag() == tag)
                result.push_back(gameObject);
        }
        return result;
    }

    void Scene::update() {}

    void Scene::processPendingDestruction() const
    {
        for (const auto& level : levels)
        {
            for (auto* region : level->getRegions())
                region->processPendingDestruction();
        }
    }

    Region* Scene::getActiveRegion() const
    {
        return activeLevel == nullptr ? nullptr : activeLevel->getActiveRegion();
    }

    std::vector<GameObject*> Scene::collectActiveGameObjects() const
    {
        auto* region = getActiveRegion();
        return region == nullptr ? std::vector<GameObject*>{} : region->getGameObjects();
    }

} // namespace engine
