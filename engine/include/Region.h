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
        [[nodiscard]] std::vector<GameObject*> getGameObjects();
        [[nodiscard]] std::vector<const GameObject*> getGameObjects() const;
        std::unique_ptr<GameObject> detach(uint64_t id);
        void adopt(std::unique_ptr<GameObject> gameObject);

    private:
        void requestDestroy(GameObject& gameObject);
        void destroyRecursive(GameObject& gameObject);
        void processPendingDestruction();

        std::string name;
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        std::vector<std::uint64_t> pendingDestruction;
    };
} // namespace engine
