#pragma once

#include "Component.h"

namespace engine
{
class Collider;
class Collision;

class BehaviourScript : public Component
{
public:
    explicit BehaviourScript(GameObject& owner) noexcept;
    ~BehaviourScript() override = default;

    void update();
    void destroy();

    virtual void onStart();
    virtual void onUpdate();
    virtual void onDestroy();
    virtual void onCollisionEnter(Collision& collision);
    virtual void onCollisionExit(Collision& collision);
    virtual void onTriggerEnter(Collider& other);
    virtual void onTriggerExit(Collider& other);

private:
    bool started{false};
};
} // namespace engine
