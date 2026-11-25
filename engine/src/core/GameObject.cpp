#include "GameObject.h"

#include "Region.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace engine
{
    namespace
    {
        bool decompose(const Matrix3& matrix, Vector2& position, float& rotation, Vector2& scale)
        {
            constexpr float epsilon = 1.0e-5F;
            position = {matrix.at(0, 2), matrix.at(1, 2)};
            const float firstColumn = std::hypot(matrix.at(0, 0), matrix.at(1, 0));
            const float secondColumn = std::hypot(matrix.at(0, 1), matrix.at(1, 1));

            if (firstColumn <= epsilon)
            {
                if (secondColumn <= epsilon)
                {
                    rotation = 0.0F;
                    scale = {0.0F, 0.0F};
                    return true;
                }
                rotation = std::atan2(-matrix.at(0, 1), matrix.at(1, 1));
                scale = {0.0F, secondColumn};
                return true;
            }

            const float determinant = matrix.at(0, 0) * matrix.at(1, 1) - matrix.at(0, 1) * matrix.at(1, 0);
            const float scaleY = determinant / firstColumn;
            rotation = std::atan2(matrix.at(1, 0), matrix.at(0, 0));
            scale = {firstColumn, scaleY};

            const float expectedM01 = -std::sin(rotation) * scaleY;
            const float expectedM11 = std::cos(rotation) * scaleY;
            return std::abs(matrix.at(0, 1) - expectedM01) <= epsilon &&
                   std::abs(matrix.at(1, 1) - expectedM11) <= epsilon;
        }
    } // namespace

    std::atomic<std::uint64_t> GameObject::nextId{1};

    GameObject::GameObject(Region& ownerRegion, std::string objectName)
        : id(nextId.fetch_add(1))
        , name(std::move(objectName))
        , region(&ownerRegion)
        , transform(*this)
    {
    }

    GameObject::~GameObject()
    {
        if (parent != nullptr)
        {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }
        for (GameObject* child : children)
        {
            if (child != nullptr && child->parent == this)
            {
                child->parent = nullptr;
            }
        }
    }

    Transform& GameObject::getTransform() noexcept
    {
        return transform;
    }

    const Transform& GameObject::getTransform() const noexcept
    {
        return transform;
    }

    Region& GameObject::getRegion() noexcept
    {
        return *region;
    }

    const Region& GameObject::getRegion() const noexcept
    {
        return *region;
    }

    bool GameObject::removeComponent(Component& component)
    {
        const auto found =
            std::find_if(components.begin(),
                         components.end(),
                         [&component](const auto& ownedComponent) { return ownedComponent.get() == &component; });
        if (found == components.end())
        {
            return false;
        }
        components.erase(found);
        return true;
    }

    std::uint64_t GameObject::getId() const noexcept
    {
        return id;
    }

    const std::string& GameObject::getName() const noexcept
    {
        return name;
    }

    void GameObject::setName(std::string newName)
    {
        name = std::move(newName);
    }

    const std::string& GameObject::getTag() const noexcept
    {
        return tag;
    }

    void GameObject::setTag(std::string_view newTag)
    {
        tag.assign(newTag);
    }

    GameObject* GameObject::getParent() const noexcept
    {
        return parent;
    }

    const std::vector<GameObject*>& GameObject::getChildren() const noexcept
    {
        return children;
    }

    bool GameObject::setParent(GameObject* newParent, bool preserveWorldTransform)
    {
        if (newParent == parent)
        {
            return true;
        }
        for (GameObject* ancestor = newParent; ancestor != nullptr; ancestor = ancestor->parent)
        {
            if (ancestor == this)
            {
                return false;
            }
        }

        Vector2 localPosition = transform.getLocalPosition();
        float localRotation = transform.getLocalRotation();
        Vector2 localScale = transform.getLocalScale();
        if (preserveWorldTransform)
        {
            Matrix3 local = transform.getWorldTransform();
            if (newParent != nullptr)
            {
                const auto inverse = newParent->getTransform().getWorldTransform().inverse();
                if (!inverse.has_value())
                {
                    return false;
                }
                local = *inverse * local;
            }
            if (!decompose(local, localPosition, localRotation, localScale))
            {
                return false;
            }
        }

        if (newParent != nullptr)
        {
            newParent->children.reserve(newParent->children.size() + 1);
        }
        if (parent != nullptr)
        {
            auto& siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }
        parent = newParent;
        if (parent != nullptr)
        {
            parent->children.push_back(this);
        }
        if (preserveWorldTransform)
        {
            transform.setLocalPosition(localPosition);
            transform.setLocalRotation(localRotation);
            transform.setLocalScale(localScale);
        }
        return true;
    }

    void GameObject::destroy() noexcept
    {
        pendingDestroy = true;
    }

    bool GameObject::isPendingDestroy() const noexcept
    {
        return pendingDestroy;
    }
} // namespace engine
