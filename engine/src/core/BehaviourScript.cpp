#include "BehaviourScript.h"

namespace engine
{
    BehaviourScript::BehaviourScript(GameObject& owner) noexcept
        : Component(owner)
    {
    }

    void BehaviourScript::update()
    {
        if (!isEnabled())
        {
            return;
        }

        if (!started)
        {
            onStart();
            started = true;
        }
        onUpdate();
    }

    void BehaviourScript::destroy()
    {
        if (started)
        {
            onDestroy();
            started = false;
        }
    }

    void BehaviourScript::onStart() {}

    void BehaviourScript::onUpdate() {}

    void BehaviourScript::onDestroy() {}

    void BehaviourScript::onCollisionEnter(Collision& collision)
    {
        static_cast<void>(collision);
    }

    void BehaviourScript::onCollisionExit(Collision& collision)
    {
        static_cast<void>(collision);
    }

    void BehaviourScript::onTriggerEnter(Collider& other)
    {
        static_cast<void>(other);
    }

    void BehaviourScript::onTriggerExit(Collider& other)
    {
        static_cast<void>(other);
    }
} // namespace engine
