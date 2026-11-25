#pragma once

#include <any>
#include <functional>
#include <typeindex>
#include <utility>
#include <vector>

namespace engine
{
class EventBus
{
public:
    class Subscription
    {
    public:
        Subscription() = default;
        explicit Subscription(std::function<void()> unsubscribe);
        ~Subscription();

        Subscription(const Subscription&) = delete;
        auto operator=(const Subscription&) -> Subscription& = delete;
        Subscription(Subscription&&) noexcept = default;
        auto operator=(Subscription&&) noexcept -> Subscription& = default;

        void unsubscribe() noexcept;

    private:
        std::function<void()> unsubscribe_;
    };

    template <typename T>
    auto subscribe(std::function<void(const T&)> callback) -> Subscription
    {
        const std::size_t index = subscribers_.size();
        subscribers_.push_back(Subscriber{
            typeid(T),
            [callback = std::move(callback)](const std::any& value) { callback(std::any_cast<const T&>(value)); },
        });
        return Subscription(
            [this, index]()
            {
                if (index < subscribers_.size())
                {
                    subscribers_[index].connected = false;
                }
            });
    }

    template <typename T>
    void publish(T event)
    {
        pending_.push_back(PendingEvent{typeid(T), std::any{std::move(event)}});
    }

    void dispatchPending();

private:
    struct Subscriber
    {
        std::type_index type;
        std::function<void(const std::any&)> invoker;
        bool connected{true};
    };

    struct PendingEvent
    {
        std::type_index type;
        std::any value;
    };

    std::vector<Subscriber> subscribers_;
    std::vector<PendingEvent> pending_;
};
} // namespace engine
