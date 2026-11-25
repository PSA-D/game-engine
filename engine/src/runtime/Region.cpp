#include "Region.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace engine
{
    namespace
    {
        // Elk GameObject in het hele proces krijgt een uniek id, ook over Regions en Levels heen.
        // Zo kan een id overal zonder verwarring opgezocht worden. 0 is ongeldig.
        std::atomic<std::uint64_t> nextGameObjectId{1};
    } // namespace

    Region::Region(std::string regionName)
        : name(std::move(regionName))
    {
    }

    Region::~Region() = default;

    // GameObject& Region::createGameObject(std::string_view objectName)
    // {
    //     const auto id = nextGameObjectId.fetch_add(1, std::memory_order_relaxed);
    //     gameObjects.push_back(std::make_unique<GameObject>(id, std::string(objectName), *this));
    //     return *gameObjects.back();
    // }
    //    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
    // void Region::requestDestroy(GameObject& gameObject)
    // {
    //     if (&gameObject.getRegion() != this)
    //         return;
    //
    //     const auto id = gameObject.getId();
    //     if (std::find(pendingDestruction.begin(), pendingDestruction.end(), id) ==
    //         pendingDestruction.end())
    //     {
    //         pendingDestruction.push_back(id);
    //     }
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

    // void Region::processPendingDestruction()
    // {
    //     // Een lus, want vernietigen kan nieuwe verzoeken veroorzaken.
    //     while (!pendingDestruction.empty())
    //     {
    //         const auto batch = std::move(pendingDestruction);
    //         pendingDestruction.clear();
    //
    //         for (const auto id : batch)
    //         {
    //             const auto found =
    //                 std::find_if(gameObjects.begin(), gameObjects.end(),
    //                              [id](const auto& owned) { return owned->getId() == id; });
    //             // Niet gevonden betekent dat het al weg is, bijvoorbeeld als kind van een ouder.
    //             if (found != gameObjects.end())
    //                 destroyRecursive(**found);
    //         }
    //     }
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

    const std::string& Region::getName() const
    {
        return name;
    }

    // std::vector<GameObject*> Region::getGameObjects()
    // {
    //     std::vector<GameObject*> result;
    //     result.reserve(gameObjects.size());
    //     for (auto& gameObject : gameObjects)
    //         result.push_back(gameObject.get());
    //     return result;
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

    // std::vector<const GameObject*> Region::getGameObjects() const
    // {
    //     std::vector<const GameObject*> result;
    //     result.reserve(gameObjects.size());
    //     for (const auto& gameObject : gameObjects)
    //         result.push_back(gameObject.get());
    //     return result;
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

    // void Region::destroyRecursive(GameObject& gameObject)
    // {
    //     // Een kopie, want het losmaken van een kind past de kinderlijst van de ouder aan.
    //     const std::vector<GameObject*> children = gameObject.getChildren();
    //     for (auto* child : children)
    //         destroyRecursive(*child);
    //
    //     // Uit de kinderlijst van zijn ouder halen. Zonder ouder lukt dit altijd.
    //     static_cast<void>(gameObject.setParent(nullptr, false));
    //
    //     // erase behoudt de volgorde van de overige objecten.
    //     const auto found = std::find_if(gameObjects.begin(), gameObjects.end(),
    //                                     [&gameObject](const auto& owned)
    //                                     { return owned.get() == &gameObject; });
    //     if (found != gameObjects.end())
    //         gameObjects.erase(found);
    // }
    // TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
} // namespace engine
