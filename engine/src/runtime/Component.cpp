#include "Component.h"

namespace engine
{
Component::Component(GameObject& ownerObject) noexcept
    : owner(ownerObject)
{
}

Component::~Component() = default;

GameObject& Component::getOwner() noexcept
{
    return owner;
}

const GameObject& Component::getOwner() const noexcept
{
    return owner;
}

bool Component::isEnabled() const noexcept
{
    return enabled;
}

void Component::setEnabled(bool newEnabled) noexcept
{
    enabled = newEnabled;
}
} // namespace engine
