#pragma once

#include "BehaviourScript.h"
#include "Component.h"
#include "Transform.h"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace engine
{
    class Region;

    class GameObject
    {
    public:
        ~GameObject();

        GameObject(const GameObject&) = delete;
        auto operator=(const GameObject&) -> GameObject& = delete;
        GameObject(GameObject&&) = delete;
        auto operator=(GameObject&&) -> GameObject& = delete;

        template <typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>);
            auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
            T& result = *component;

            if constexpr (std::is_base_of_v<BehaviourScript, T>)
                behaviourScripts.push_back(&result);

            components.push_back(std::move(component));
            return result;
        }

        template <typename T>
        [[nodiscard]] T* getComponent(std::size_t idx = 0) noexcept
        {
            static_assert(std::is_base_of_v<Component, T>);
            for (const auto& component : components)
            {
                if (auto* typedComponent = dynamic_cast<T*>(component.get()))
                {
                    if (idx == 0)
                    {
                        return typedComponent;
                    }
                    --idx;
                }
            }
            return nullptr;
        }

        template <typename T>
        [[nodiscard]] const T* getComponent(std::size_t idx = 0) const noexcept
        {
            static_assert(std::is_base_of_v<Component, T>);
            for (const auto& component : components)
            {
                if (const auto* typedComponent = dynamic_cast<const T*>(component.get()))
                {
                    if (idx == 0)
                    {
                        return typedComponent;
                    }
                    --idx;
                }
            }
            return nullptr;
        }

        template <typename T>
        [[nodiscard]] std::vector<T*> getComponents()
        {
            static_assert(std::is_base_of_v<Component, T>);
            std::vector<T*> result;
            for (const auto& component : components)
            {
                if (auto* typedComponent = dynamic_cast<T*>(component.get()))
                {
                    result.push_back(typedComponent);
                }
            }
            return result;
        }

        template <typename T>
        [[nodiscard]] std::vector<const T*> getComponents() const
        {
            static_assert(std::is_base_of_v<Component, T>);
            std::vector<const T*> result;
            for (const auto& component : components)
            {
                if (const auto* typedComponent = dynamic_cast<const T*>(component.get()))
                {
                    result.push_back(typedComponent);
                }
            }
            return result;
        }

        template <typename T>
        [[nodiscard]] bool hasComponent() const noexcept
        {
            return getComponent<T>() != nullptr;
        }

        template <typename T>
        [[nodiscard]] bool removeComponent(std::size_t idx = 0)
        {
            static_assert(std::is_base_of_v<Component, T>);
            for (auto component = components.begin(); component != components.end(); ++component)
            {
                if (dynamic_cast<T*>(component->get()) != nullptr)
                {
                    if (idx == 0)
                    {
                        unregisterBehaviour(component->get());
                        components.erase(component);
                        return true;
                    }
                    --idx;
                }
            }
            return false;
        }

        [[nodiscard]] bool removeComponent(Component& component);

        template <typename T>
        [[nodiscard]] std::size_t removeComponents()
        {
            static_assert(std::is_base_of_v<Component, T>);
            for (const auto& component : components)
            {
                if (dynamic_cast<T*>(component.get()) != nullptr)
                    unregisterBehaviour(component.get());
            }

            const auto oldSize = components.size();
            components.erase(std::remove_if(components.begin(),
                                            components.end(),
                                            [](const auto& component)
                                            { return dynamic_cast<T*>(component.get()) != nullptr; }),
                             components.end());
            return oldSize - components.size();
        }

        [[nodiscard]] Transform& getTransform() noexcept;
        [[nodiscard]] const Transform& getTransform() const noexcept;

        [[nodiscard]] Region& getRegion() noexcept;
        [[nodiscard]] const Region& getRegion() const noexcept;
        [[nodiscard]] std::uint64_t getId() const noexcept;
        [[nodiscard]] const std::string& getName() const noexcept;
        void setName(std::string newName);
        [[nodiscard]] const std::string& getTag() const noexcept;
        void setTag(std::string_view newTag);

        [[nodiscard]] GameObject* getParent() const noexcept;
        [[nodiscard]] const std::vector<GameObject*>& getChildren() const noexcept;
        [[nodiscard]] bool setParent(GameObject* newParent, bool preserveWorldTransform = true);

        void destroy() noexcept;
        [[nodiscard]] bool isPendingDestroy() const noexcept;

        void updateBehaviours() const;

    private:
        friend class Region;

        GameObject(Region& ownerRegion, std::string objectName);
        void unregisterBehaviour(Component* component) noexcept;

        static std::atomic<std::uint64_t> nextId;

        std::uint64_t id;
        std::string name;
        std::string tag;
        Region* region;
        GameObject* parent{nullptr};
        std::vector<GameObject*> children;
        Transform transform;
        std::vector<std::unique_ptr<Component>> components;
        // Look-up vector voor BehaviourScripts.
        // Hierdoor hoeft er geen dynamic_cast<> in elke frame aangeroepen te worden
        // en hoeft niet elke component te worden langs gegaan.
        std::vector<BehaviourScript*> behaviourScripts;
        bool pendingDestroy{false};
    };
} // namespace engine
