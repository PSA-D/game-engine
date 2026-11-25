#include "BehaviourScript.h"
#include "GameObject.h"
#include "Region.h"

#include <cstdlib>
#include <iostream>

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

auto runBehaviourScriptTests() -> int
{
    int failures = 0;
    const auto check = [&failures](bool condition, const char* message)
    {
        if (!condition)
        {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    std::cout << "Running BehaviourScript tests\n";
    engine::Region region;
    engine::GameObject& gameObject = region.createGameObject();
    CountingBehaviour& behaviour = gameObject.addComponent<CountingBehaviour>();

    check(&behaviour.getOwner() == &gameObject, "BehaviourScript is attached to its GameObject");
    behaviour.update();
    check(behaviour.startCount == 1, "onStart runs before the first update");
    check(behaviour.updateCount == 1, "onUpdate runs during update");

    behaviour.update();
    check(behaviour.startCount == 1, "onStart runs only once");
    check(behaviour.updateCount == 2, "onUpdate runs on each update");

    behaviour.destroy();
    check(behaviour.destroyCount == 1, "onDestroy runs when the script is destroyed");

    behaviour.setEnabled(false);
    behaviour.update();
    check(behaviour.updateCount == 2, "disabled BehaviourScript does not update");

    if (failures == 0)
    {
        std::cout << "All BehaviourScript tests passed\n";
    }
    else
    {
        std::cerr << failures << " BehaviourScript test(s) failed\n";
    }

    return failures;
}
