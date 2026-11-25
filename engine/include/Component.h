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

        [[nodiscard]] bool isEnabled() const noexcept;
        void setEnabled(bool enabled) noexcept;

    private:
        GameObject& owner;
        bool enabled{true};
    };
} // namespace engine
