#include "EventBus.h"

namespace engine
{
EventBus::Subscription::Subscription(std::function<void()> unsubscribe)
    : unsubscribe_(std::move(unsubscribe))
{
}

EventBus::Subscription::~Subscription()
{
    unsubscribe();
}

auto EventBus::Subscription::unsubscribe() noexcept -> void
{
    if (unsubscribe_)
    {
        unsubscribe_();
        unsubscribe_ = nullptr;
    }
}

auto EventBus::dispatchPending() -> void
{
    std::vector<PendingEvent> current;
    current.swap(pending_);
    for (const auto& event : current)
    {
        for (auto& subscriber : subscribers_)
        {
            if (subscriber.connected && subscriber.type == event.type)
            {
                subscriber.invoker(event.value);
            }
        }
    }
}
} // namespace engine
