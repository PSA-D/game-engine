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
    Region() = default;
    ~Region();

    Region(const Region&) = delete;
    Region& operator=(const Region&) = delete;
    Region(Region&&) = delete;
    Region& operator=(Region&&) = delete;

    GameObject& createGameObject(std::string name = {});

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};
} // namespace engine
