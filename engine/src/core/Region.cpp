#include "Region.h"

#include "GameObject.h"

#include <memory>
#include <utility>

namespace engine
{
    Region::~Region() = default;

    GameObject& Region::createGameObject(std::string name)
    {
        auto gameObject = std::unique_ptr<GameObject>(new GameObject(*this, std::move(name)));
        GameObject& result = *gameObject;
        gameObjects.push_back(std::move(gameObject));
        return result;
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
                const auto found =
                    std::find_if(gameObjects.begin(), gameObjects.end(),
                                 [id](const auto& owned) { return owned->getId() == id; });
                // Niet gevonden betekent dat het al weg is, bijvoorbeeld als kind van een ouder.
                if (found != gameObjects.end())
                    destroyRecursive(**found);
            }
        }
    }

    std::vector<GameObject*> Region::getGameObjects()
    {
        std::vector<GameObject*> result;
        result.reserve(gameObjects.size());
        for (auto& gameObject : gameObjects)
            result.push_back(gameObject.get());
        return result;
    }

    std::vector<const GameObject*> Region::getGameObjects() const
    {
        std::vector<const GameObject*> result;
        result.reserve(gameObjects.size());
        for (const auto& gameObject : gameObjects)
            result.push_back(gameObject.get());
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
        const auto found = std::find_if(gameObjects.begin(), gameObjects.end(),
                                        [&gameObject](const auto& owned)
                                        { return owned.get() == &gameObject; });
        if (found != gameObjects.end())
            gameObjects.erase(found);
    }
} // namespace engine
