#pragma once

namespace engine
{
class GameObject;

class Component
{
public:
    explicit Component(GameObject& owner) noexcept;
    virtual ~Component();

    [[nodiscard]] GameObject& getOwner() noexcept;
    [[nodiscard]] const GameObject& getOwner() const noexcept;

    bool enabled{true};

private:
    GameObject& owner;
};
} // namespace engine
