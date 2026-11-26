#include "runtime/GameObject.h"
#include "runtime/Region.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>

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

TEST_CASE("GameObject vindt componenten op type en index", "[unit][game-object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();
    auto& first = object.addComponent<TestComponent>(1);
    auto& second = object.addComponent<TestComponent>(2);

    CHECK(object.getComponent<TestComponent>() == &first);
    CHECK(object.getComponent<TestComponent>(1) == &second);
    CHECK(object.hasComponent<TestComponent>());
    CHECK(object.getTransform().getOwner().getId() == object.getId()); // GameObject bezit een Transform
    CHECK(object.getComponents<TestComponent>().size() == 2);

    SECTION("uitgeschakelde componenten kun je overslaan")
    {
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

        CHECK(enabledCount == 1);
    }
}

TEST_CASE("GameObject verwijdert componenten", "[unit][game-object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();
    bool destroyed = false;
    auto& first = object.addComponent<TestComponent>(1, &destroyed);
    object.addComponent<TestComponent>(2);
    object.addComponent<TestComponent>(3);

    CHECK(object.removeComponent<TestComponent>(1));          // op type en index
    CHECK(object.getComponent<TestComponent>(1)->value == 3); // de rest blijft staan
    CHECK(object.removeComponent(first));                     // op referentie
    CHECK(destroyed);                                         // destructor is aangeroepen
    CHECK(object.getComponent<TestComponent>()->value == 3);
    CHECK(object.removeComponents<TestComponent>() == 1); // alle overgebleven
    CHECK_FALSE(object.hasComponent<TestComponent>());
}

TEST_CASE("GameObject transform en hiërarchie", "[unit][game-object]")
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

    CHECK(child.getTransform().getLocalPosition() == engine::Vector2{4.0F, 5.0F});
    CHECK(child.getTransform().getLocalRotation() == Catch::Approx(-0.3F).margin(1.0e-4F));
    CHECK(child.getTransform().getLocalScale() == engine::Vector2{1.5F, 0.75F});
    CHECK(&child.getRegion() == &region);

    const engine::Matrix3 worldTransform = child.getTransform().getWorldTransform();

    REQUIRE(child.setParent(&parent));
    CHECK(nearlyEqual(worldTransform, child.getTransform().getWorldTransform())); // wereldtransform blijft gelijk
    CHECK(child.getTransform().getWorldRotation() == Catch::Approx(-0.3F).margin(1.0e-4F));
    CHECK(parent.getChildren().size() == 1);
    CHECK(child.getParent() == &parent);

    SECTION("loskoppelen haalt de ouder-kindrelatie weg")
    {
        CHECK(child.setParent(nullptr));
        CHECK(parent.getChildren().empty());
    }
}

TEST_CASE("GameObject::destroy markeert alleen voor uitgesteld opruimen", "[unit][game-object]")
{
    engine::Region region;
    engine::GameObject& object = region.createGameObject();

    object.destroy();

    CHECK(object.isPendingDestroy());
}

TEST_CASE("GameObject weigert een cyclus in de hiërarchie", "[unit][game-object]")
{
    engine::Region region;
    engine::GameObject& parent = region.createGameObject();
    engine::GameObject& child = region.createGameObject();

    CHECK(child.setParent(&parent, false));
    CHECK_FALSE(parent.setParent(&child, false));
    CHECK(parent.getChildren().size() == 1); // bestaande hiërarchie blijft ongewijzigd
}
