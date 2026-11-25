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
} // namespace engine
