#pragma once

#include "core/Component.h"

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace engine
{
class GameObject
{
public:
    GameObject() = default;
    ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;

    template <typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>);
        auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& result = *component;
        components.push_back(std::move(component));
        return result;
    }

    template <typename T>
    [[nodiscard]] T* getComponent() noexcept
    {
        static_assert(std::is_base_of_v<Component, T>);
        for (const auto& component : components)
        {
            if (auto* typedComponent = dynamic_cast<T*>(component.get()))
            {
                return typedComponent;
            }
        }
        return nullptr;
    }

    template <typename T>
    [[nodiscard]] const T* getComponent() const noexcept
    {
        static_assert(std::is_base_of_v<Component, T>);
        for (const auto& component : components)
        {
            if (const auto* typedComponent = dynamic_cast<const T*>(component.get()))
            {
                return typedComponent;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<Component>> components;
};
} // namespace engine
