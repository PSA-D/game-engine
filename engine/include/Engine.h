#pragma once

namespace engine
{
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    auto operator=(const Engine&) -> Engine& = delete;
    Engine(Engine&&) noexcept = default;
    auto operator=(Engine&&) noexcept -> Engine& = default;

    void init();
    void shutdown();
    [[nodiscard]] bool isRunning() const;

private:
    bool running{false};
};
} // namespace engine
