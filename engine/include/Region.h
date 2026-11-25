#pragma once

#include <memory>
#include <string>
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

    [[nodiscard]] const std::string& getName() const noexcept;
    GameObject& createGameObject(std::string objectName = {});

private:
    std::string name;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};
} // namespace engine
