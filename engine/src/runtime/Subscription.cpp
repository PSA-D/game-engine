#include "runtime/Subscription.h"

namespace engine
{
    Subscription::Subscription(std::function<void()> unsubscribe)
        : unsubscribe_(std::move(unsubscribe))
    {
    }

    Subscription::~Subscription()
    {
        unsubscribe();
    }

    void Subscription::unsubscribe() noexcept
    {
        if (unsubscribe_)
        {
            unsubscribe_();
            unsubscribe_ = nullptr;
        }
    }
} // namespace engine
