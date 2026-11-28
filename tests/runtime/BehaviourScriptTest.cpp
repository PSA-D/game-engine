#include "runtime/BehaviourScript.h"

#include "runtime/GameObject.h"
#include "runtime/Region.h"

#include <catch2/catch_test_macros.hpp>

namespace
{
    class CountingBehaviour final : public engine::BehaviourScript
    {
    public:
        using BehaviourScript::BehaviourScript;

        int startCount{0};
        int updateCount{0};
        int destroyCount{0};

        void onStart() override
        {
            ++startCount;
        }

        void onUpdate() override
        {
            ++updateCount;
        }

        void onDestroy() override
        {
            ++destroyCount;
        }
    };
} // namespace

TEST_CASE("BehaviourScript lifecycle hooks run in expected order", "[unit][runtime]")
{
    engine::Region region;
    engine::GameObject& gameObject = region.createGameObject();
    auto& behaviour = gameObject.addComponent<CountingBehaviour>();

    SECTION("BehaviourScript is attached to its GameObject")
    {
        CHECK(&behaviour.getOwner() == &gameObject);
    }

    SECTION("onStart runs once before the first update, while onUpdate runs every frame")
    {
        behaviour.update();
        CHECK(behaviour.startCount == 1);
        CHECK(behaviour.updateCount == 1);

        behaviour.update();
        CHECK(behaviour.startCount == 1);
        CHECK(behaviour.updateCount == 2);
    }

    SECTION("onDestroy does not run if the script was never started")
    {
        behaviour.destroy();
        CHECK(behaviour.destroyCount == 0);
    }

    SECTION("onDestroy runs when a started script is destroyed")
    {
        behaviour.update(); // Triggers onStart()
        REQUIRE(behaviour.startCount == 1);

        behaviour.destroy();
        CHECK(behaviour.destroyCount == 1);
    }

    SECTION("Disabled BehaviourScript does not update")
    {
        behaviour.update();
        REQUIRE(behaviour.updateCount == 1);

        behaviour.setEnabled(false);
        behaviour.update();
        CHECK(behaviour.updateCount == 1);
    }
}
