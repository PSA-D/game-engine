#include "GameObject.h"
#include "Region.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace
{
class TestComponent final : public engine::Component
{
public:
    TestComponent(engine::GameObject& ownerObject, int componentValue, bool* destroyedFlag = nullptr)
        : Component(ownerObject)
        , value(componentValue)
        , destroyed(destroyedFlag)
    {
    }

    ~TestComponent() override
    {
        if (destroyed != nullptr)
        {
            *destroyed = true;
        }
    }

    int value;
    bool* destroyed;
};

auto nearlyEqual(float first, float second) -> bool
{
    return std::abs(first - second) < 1.0e-4F;
}

auto nearlyEqual(const engine::Matrix3& first, const engine::Matrix3& second) -> bool
{
    for (std::uint32_t row = 0; row < 3; ++row)
    {
        for (std::uint32_t column = 0; column < 3; ++column)
        {
            if (!nearlyEqual(first.at(row, column), second.at(row, column)))
            {
                return false;
            }
        }
    }
    return true;
}
} // namespace

auto runGameObjectTests() -> int
{
    int failures = 0;
    std::cout << "Running GameObject tests\n";
    const auto check = [&failures](bool condition, const char* message)
    {
        if (!condition)
        {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    {
        const int groupFailures = failures;
        engine::Region region;
        engine::GameObject& object = region.createGameObject();
        auto& first = object.addComponent<TestComponent>(1);
        auto& second = object.addComponent<TestComponent>(2);
        check(object.getComponent<TestComponent>() == &first, "first component is found");
        check(object.getComponent<TestComponent>(1) == &second, "second component is found by index");
        check(object.hasComponent<TestComponent>(), "component type is present");
        check(object.getTransform().getOwner().getId() == object.getId(), "GameObject owns a Transform");
        check(object.getComponents<TestComponent>().size() == 2, "getComponents returns all matching components");

        second.setEnabled(false);
        std::size_t enabledCount = 0;
        for (std::size_t index = 0;; ++index)
        {
            engine::Component* component = object.getComponent<engine::Component>(index);
            if (component == nullptr)
            {
                break;
            }
            if (component->isEnabled())
            {
                ++enabledCount;
            }
        }
        check(enabledCount == 1, "component consumer skips disabled components");
        if (failures == groupFailures)
        {
            std::cout << "GameObject component lookup tests passed\n";
        }
    }

    {
        const int groupFailures = failures;
        engine::Region region;
        engine::GameObject& object = region.createGameObject();
        bool destroyed = false;
        auto& first = object.addComponent<TestComponent>(1, &destroyed);
        object.addComponent<TestComponent>(2);
        object.addComponent<TestComponent>(3);
        check(object.removeComponent<TestComponent>(1), "component is removed by type and index");
        check(object.getComponent<TestComponent>(1)->value == 3, "indexed removal keeps other components");
        check(object.removeComponent(first), "component is removed by reference");
        check(destroyed, "removed component destructor is called");
        check(object.getComponent<TestComponent>()->value == 3, "remaining component can still be found");
        check(object.removeComponents<TestComponent>() == 1, "removeComponents removes all remaining matches");
        check(!object.hasComponent<TestComponent>(), "no matching components remain");
        if (failures == groupFailures)
        {
            std::cout << "GameObject component removal tests passed\n";
        }
    }

    {
        const int groupFailures = failures;
        engine::Region region;
        engine::GameObject& parent = region.createGameObject();
        engine::GameObject& child = region.createGameObject();
        parent.getTransform().setLocalPosition({10.0F, 20.0F});
        parent.getTransform().setLocalRotation(0.4F);
        parent.getTransform().setLocalScale({2.0F, 2.0F});
        child.getTransform().setLocalPosition({4.0F, 5.0F});
        child.getTransform().setLocalRotation(-0.3F);
        child.getTransform().setLocalScale({1.5F, 0.75F});
        check(child.getTransform().getLocalPosition() == engine::Vector2{4.0F, 5.0F}, "local position has a getter");
        check(nearlyEqual(child.getTransform().getLocalRotation(), -0.3F), "local rotation has a getter");
        check(child.getTransform().getLocalScale() == engine::Vector2{1.5F, 0.75F}, "local scale has a getter");
        const engine::Matrix3 worldTransform = child.getTransform().getWorldTransform();
        check(&child.getRegion() == &region, "GameObject refers to its Region");
        check(child.setParent(&parent), "setParent accepts a valid parent");
        check(nearlyEqual(worldTransform, child.getTransform().getWorldTransform()),
              "parenting preserves the representable world transform");
        check(nearlyEqual(child.getTransform().getWorldRotation(), -0.3F),
              "world rotation is computed from the parent and local transforms");
        check(parent.getChildren().size() == 1 && child.getParent() == &parent, "hierarchy links are updated");

        check(child.setParent(nullptr), "setParent can detach an object");
        check(parent.getChildren().empty(), "detaching removes the child link");
        if (failures == groupFailures)
        {
            std::cout << "GameObject transform and hierarchy tests passed\n";
        }
    }

    {
        const int groupFailures = failures;
        engine::Region region;
        engine::GameObject& object = region.createGameObject();
        object.destroy();
        check(object.isPendingDestroy(), "destroy only marks the object for deferred removal");
        if (failures == groupFailures)
        {
            std::cout << "GameObject deferred-destruction tests passed\n";
        }
    }

    {
        const int groupFailures = failures;
        engine::Region region;
        engine::GameObject& parent = region.createGameObject();
        engine::GameObject& child = region.createGameObject();
        check(child.setParent(&parent, false), "setParent changes hierarchy without preserving transforms");
        check(!parent.setParent(&child, false), "setParent rejects a hierarchy cycle");
        check(parent.getChildren().size() == 1, "failed reparenting leaves existing hierarchy unchanged");
        if (failures == groupFailures)
        {
            std::cout << "GameObject hierarchy validation tests passed\n";
        }
    }

    if (failures == 0)
    {
        std::cout << "All GameObject tests passed\n";
    }
    else
    {
        std::cerr << failures << " GameObject test(s) failed\n";
    }

    return failures;
}
