#include "Transform.h"

#include "GameObject.h"

#include <cmath>

namespace engine
{
    Transform::Transform(GameObject& ownerObject) noexcept
        : Component(ownerObject)
    {
    }

    void Transform::setLocalPosition(const Vector2& position) noexcept
    {
        localPosition = position;
    }

    const Vector2& Transform::getLocalPosition() const noexcept
    {
        return localPosition;
    }

    void Transform::setLocalRotation(float rotationRadians) noexcept
    {
        localRotation = rotationRadians;
    }

    float Transform::getLocalRotation() const noexcept
    {
        return localRotation;
    }

    void Transform::setLocalScale(const Vector2& scale) noexcept
    {
        localScale = scale;
    }

    const Vector2& Transform::getLocalScale() const noexcept
    {
        return localScale;
    }

    Vector2 Transform::getWorldPosition() const
    {
        const Matrix3 world = getWorldTransform();
        return {world.at(0, 2), world.at(1, 2)};
    }

    float Transform::getWorldRotation() const
    {
        const Matrix3 world = getWorldTransform();
        return std::atan2(world.at(1, 0), world.at(0, 0));
    }

    Vector2 Transform::getWorldScale() const
    {
        const Matrix3 world = getWorldTransform();
        const float scaleX = std::hypot(world.at(0, 0), world.at(1, 0));
        const float scaleY = std::hypot(world.at(0, 1), world.at(1, 1));
        if (scaleX == 0.0F)
        {
            return {0.0F, scaleY};
        }
        const float determinant = world.at(0, 0) * world.at(1, 1) - world.at(0, 1) * world.at(1, 0);
        return {scaleX, determinant / scaleX};
    }

    Matrix3 Transform::getWorldTransform() const
    {
        const float cosine = std::cos(localRotation);
        const float sine = std::sin(localRotation);
        const Matrix3 local{std::array<float, Matrix3::ARRAY_SIZE>{
            cosine * localScale.x,
            -sine * localScale.y,
            localPosition.x,
            sine * localScale.x,
            cosine * localScale.y,
            localPosition.y,
            0.0F,
            0.0F,
            1.0F,
        }};

        const GameObject* parent = getOwner().getParent();
        if (parent == nullptr)
        {
            return local;
        }
        return parent->getTransform().getWorldTransform() * local;
    }
} // namespace engine
