#pragma once

#include <functional>

namespace engine
{
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
} // namespace engine
