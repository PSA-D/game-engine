#pragma once

#include "Component.h"
#include "utils/Matrix.h"
#include "utils/Vector2.h"

namespace engine
{
class Transform final : public Component
{
public:
    explicit Transform(GameObject& owner) noexcept;

    void setLocalPosition(const Vector2& position) noexcept;
    [[nodiscard]] const Vector2& getLocalPosition() const noexcept;
    void setLocalRotation(float rotationRadians) noexcept;
    [[nodiscard]] float getLocalRotation() const noexcept;
    void setLocalScale(const Vector2& scale) noexcept;
    [[nodiscard]] const Vector2& getLocalScale() const noexcept;

    [[nodiscard]] Vector2 getWorldPosition() const;
    [[nodiscard]] float getWorldRotation() const;
    [[nodiscard]] Vector2 getWorldScale() const;
    [[nodiscard]] Matrix3 getWorldTransform() const;

private:
    Vector2 localPosition{};
    float localRotation{0.0F};
    Vector2 localScale{1.0F, 1.0F};
};
} // namespace engine
