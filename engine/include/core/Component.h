#pragma once

namespace engine
{
class GameObject;

class Component
{
public:
    explicit Component(GameObject& ownerObject) noexcept
        : owner(ownerObject)
    {
    }

    virtual ~Component() = default;

    [[nodiscard]] GameObject& getOwner() noexcept
    {
        return owner;
    }

    [[nodiscard]] const GameObject& getOwner() const noexcept
    {
        return owner;
    }

    [[nodiscard]] bool isEnabled() const noexcept
    {
        return enabled;
    }

    void setEnabled(bool value) noexcept
    {
        enabled = value;
    }

private:
    GameObject& owner;
    bool enabled{true};
};
} // namespace engine
