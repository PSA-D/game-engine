#include "GameObject.h"
#include "Region.h"

#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>

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

auto nearlyEqual(const engine::Matrix& first, const engine::Matrix& second) -> bool
{
    for (std::size_t row = 0; row < 3; ++row)
    {
        for (std::size_t column = 0; column < 3; ++column)
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

TEST_CASE("GameObject component lookup", "[game_object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();
    auto& first = object.addComponent<TestComponent>(1);
    auto& second = object.addComponent<TestComponent>(2);

    REQUIRE(object.getComponent<TestComponent>() == &first);
    REQUIRE(object.getComponent<TestComponent>(1) == &second);
    REQUIRE(object.hasComponent<TestComponent>());
    REQUIRE(object.getTransform().getOwner().getId() == object.getId());
    REQUIRE(object.getComponents<TestComponent>().size() == 2);

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
    REQUIRE(enabledCount == 1);
}

TEST_CASE("GameObject component removal", "[game_object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();
    bool destroyed = false;
    auto& first = object.addComponent<TestComponent>(1, &destroyed);
    object.addComponent<TestComponent>(2);
    object.addComponent<TestComponent>(3);

    REQUIRE(object.removeComponent<TestComponent>(1));
    REQUIRE(object.getComponent<TestComponent>(1)->value == 3);
    REQUIRE(object.removeComponent(first));
    REQUIRE(destroyed);
    REQUIRE(object.getComponent<TestComponent>()->value == 3);
    REQUIRE(object.removeComponents<TestComponent>() == 1);
    REQUIRE(!object.hasComponent<TestComponent>());
}

TEST_CASE("GameObject transform and hierarchy", "[game_object]")
{
    engine::Region region;
    engine::GameObject& parent = region.createGameObject();
    engine::GameObject& child = region.createGameObject();
    parent.getTransform().setLocalPosition({10.0F, 20.0F});
    parent.getTransform().setLocalRotation(0.4F);
    parent.getTransform().setLocalScale({2.0F, 2.0F});
    child.getTransform().setLocalPosition({4.0F, 5.0F});
    child.getTransform().setLocalRotation(-0.3F);
    child.getTransform().setLocalScale({1.5F, 0.75F});

    REQUIRE(child.getTransform().getLocalPosition() == engine::Vector2{4.0F, 5.0F});
    REQUIRE(nearlyEqual(child.getTransform().getLocalRotation(), -0.3F));
    REQUIRE(child.getTransform().getLocalScale() == engine::Vector2{1.5F, 0.75F});
    const engine::Matrix worldTransform = child.getTransform().getWorldTransform();
    REQUIRE(&child.getRegion() == &region);
    REQUIRE(child.setParent(&parent));
    REQUIRE(nearlyEqual(worldTransform, child.getTransform().getWorldTransform()));
    REQUIRE(nearlyEqual(child.getTransform().getWorldRotation(), -0.3F));
    REQUIRE(parent.getChildren().size() == 1);
    REQUIRE(child.getParent() == &parent);

    REQUIRE(child.setParent(nullptr));
    REQUIRE(parent.getChildren().empty());
}

TEST_CASE("GameObject deferred destruction", "[game_object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();
    object.destroy();
    REQUIRE(object.isPendingDestroy());
}

TEST_CASE("GameObject hierarchy validation", "[game_object]")
{
    engine::Region region;
    engine::GameObject& parent = region.createGameObject();
    engine::GameObject& child = region.createGameObject();

    REQUIRE(child.setParent(&parent, false));
    REQUIRE(!parent.setParent(&child, false));
    REQUIRE(parent.getChildren().size() == 1);
}
