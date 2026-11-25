#pragma once

namespace engine
{
class Scene;

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
    void run();
    [[nodiscard]] bool isRunning() const noexcept;
    void setActiveScene(Scene& scene);

private:
    bool running{false};
    Scene* activeScene_{nullptr};
};
} // namespace engine
