#include "runtime/Region.h"

#include "runtime/GameObject.h"

#include <algorithm>
#include <string>
#include <utility>

namespace engine
{
    Region::Region(std::string regionName)
        : name(std::move(regionName))
    {
    }

    Region::~Region() = default;

    GameObject& Region::createGameObject(std::string_view objectName)
    {
        auto gameObject = std::unique_ptr<GameObject>(new GameObject(*this, std::string(objectName)));
        GameObject& result = *gameObject;
        gameObjects.push_back(std::move(gameObject));
        return result;
    }

    void Region::requestDestroy(GameObject& gameObject)
    {
        if (&gameObject.getRegion() != this)
            return;

        const auto id = gameObject.getId();
        if (std::find(pendingDestruction.begin(), pendingDestruction.end(), id) == pendingDestruction.end())
        {
            pendingDestruction.push_back(id);
        }
    }

    void Region::processPendingDestruction()
    {
        // Een lus, want vernietigen kan nieuwe verzoeken veroorzaken.
        while (!pendingDestruction.empty())
        {
            const auto batch = std::move(pendingDestruction);
            pendingDestruction.clear();

            for (const auto id : batch)
            {
                const auto found = std::find_if(
                    gameObjects.begin(), gameObjects.end(), [id](const auto& owned) { return owned->getId() == id; });
                // Niet gevonden betekent dat het al weg is, bijvoorbeeld als kind van een ouder.
                if (found != gameObjects.end())
                    destroyRecursive(**found);
            }
        }
    }

    const std::string& Region::getName() const
    {
        return name;
    }

    std::vector<GameObject*> Region::getGameObjects()
    {
        std::vector<GameObject*> result;
        result.reserve(gameObjects.size());
        for (auto& gameObject : gameObjects)
        {
            if (!gameObject->isPendingDestroy())
                result.push_back(gameObject.get());
        }
        return result;
    }

    std::vector<const GameObject*> Region::getGameObjects() const
    {
        std::vector<const GameObject*> result;
        result.reserve(gameObjects.size());
        for (const auto& gameObject : gameObjects)
        {
            if (!gameObject->isPendingDestroy())
                result.push_back(gameObject.get());
        }
        return result;
    }

    void Region::destroyRecursive(GameObject& gameObject)
    {
        // Een kopie, want het losmaken van een kind past de kinderlijst van de ouder aan.
        const std::vector<GameObject*> children = gameObject.getChildren();
        for (auto* child : children)
            destroyRecursive(*child);

        // Uit de kinderlijst van zijn ouder halen. Zonder ouder lukt dit altijd.
        static_cast<void>(gameObject.setParent(nullptr, false));

        // erase behoudt de volgorde van de overige objecten.
        const auto found = std::find_if(gameObjects.begin(),
                                        gameObjects.end(),
                                        [&gameObject](const auto& owned) { return owned.get() == &gameObject; });
        if (found != gameObjects.end())
            gameObjects.erase(found);
    }

    GameObject* Region::findGameObject(std::uint64_t id)
    {
        const auto found = std::find_if(gameObjects.begin(), gameObjects.end(), [id](const auto& owned) {
            return owned->getId() == id && !owned->isPendingDestroy();
        });

        return found != gameObjects.end() ? found->get() : nullptr;
    }

    GameObject* Region::findGameObjectByName(std::string name)
    {
        const auto found = std::find_if(gameObjects.begin(), gameObjects.end(), [name](const auto& owned) {
            return owned->getName() == name && !owned->isPendingDestroy();
        });

        return found != gameObjects.end() ? found->get() : nullptr;
    }

    void Region::adopt(std::unique_ptr<GameObject> gameObject)
    {
        gameObjects.push_back(std::move(gameObject));
    }

    std::unique_ptr<GameObject> Region::detach(uint64_t id)
    {
        const auto found =
            std::find_if(gameObjects.begin(), gameObjects.end(), [id](const auto& go) { return go->getId() == id; });

        if (found == gameObjects.end())
            return nullptr;

        std::unique_ptr<GameObject> gameObject = std::move(*found);
        gameObjects.erase(found);
        return gameObject;
    }
} // namespace engine
