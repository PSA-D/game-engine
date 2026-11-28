#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
    class GameObject;

    class Region
    {
    public:
        explicit Region(std::string regionName = {});
        ~Region();

        Region(const Region&) = delete;
        auto operator=(const Region&) -> Region& = delete;
        Region(Region&&) = delete;
        auto operator=(Region&&) -> Region& = delete;

        [[nodiscard]] const std::string& getName() const;

        GameObject& createGameObject(std::string_view objectName = {});
        // In volgorde van aanmaken. Objecten die op vernietiging wachten staan er nog in.
        [[nodiscard]] std::vector<GameObject*> getGameObjects();
        [[nodiscard]] std::vector<const GameObject*> getGameObjects() const;

        GameObject* findGameObject(std::uint64_t id);
        GameObject* findGameObjectByName(std::string name);

        std::unique_ptr<GameObject> detach(uint64_t id);
        void adopt(std::unique_ptr<GameObject> gameObject);

        // Zet een object op de lijst om aan het eind van het frame te vernietigen.
        void requestDestroy(GameObject& gameObject);
        // Vernietigt alle aangevraagde objecten (met hun kinderen). Aan het eind van een frame aanroepen.
        void processPendingDestruction();

    private:
        // Vernietigt eerst alle kinderen, daarna het object zelf.
        void destroyRecursive(GameObject& gameObject);

        std::string name;
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        std::vector<std::uint64_t> pendingDestruction;
    };
} // namespace engine
