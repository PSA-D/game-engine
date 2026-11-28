#include "runtime/EventBus.h"

namespace engine
{
    void EventBus::dispatchPending()
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
