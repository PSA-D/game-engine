#pragma once

#include "GameObject.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace engine
{
class Region
{
public:
    Region() = default;
    ~Region();

    Region(const Region&) = delete;
    Region& operator=(const Region&) = delete;
    Region(Region&&) = delete;
    Region& operator=(Region&&) = delete;

    GameObject& createGameObject(std::string name = {});
    void requestDestroy(GameObject& gameObject);
    [[nodiscard]] GameObject* detach(std::uint64_t id);
    void adopt(std::unique_ptr<GameObject> gameObject);
    void processPendingDestruction();

    [[nodiscard]] std::vector<GameObject*> getGameObjects() noexcept;
    [[nodiscard]] std::vector<const GameObject*> getGameObjects() const noexcept;

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::uint64_t> pendingDestruction;
};
} // namespace engine
