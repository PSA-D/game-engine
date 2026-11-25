#pragma once

#include "Region.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
class GameObject;

class Scene
{
public:
    Scene(std::uint64_t id, std::string name);
    ~Scene();

    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = delete;
    auto operator=(Scene&&) noexcept -> Scene& = delete;

    void init();
    void shutdown();
    [[nodiscard]] bool isActive() const noexcept;

    void update();
    void render();
    void processPendingDestruction();

    [[nodiscard]] std::uint64_t getId() const noexcept;
    [[nodiscard]] const std::string& getName() const noexcept;

    GameObject& createGameObject(std::string name = {});
    [[nodiscard]] bool removeGameObject(std::uint64_t id) noexcept;
    [[nodiscard]] GameObject* findGameObject(std::uint64_t id) noexcept;
    [[nodiscard]] const GameObject* findGameObject(std::uint64_t id) const noexcept;
    [[nodiscard]] GameObject* find(std::string_view name) noexcept;
    [[nodiscard]] const GameObject* find(std::string_view name) const noexcept;

private:
    Region region;
    std::uint64_t id;
    std::string name;
    bool active{false};
    std::vector<GameObject*> gameObjects;
};
} // namespace engine
