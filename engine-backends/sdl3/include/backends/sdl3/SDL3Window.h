#pragma once

#include <string>

namespace engine::backends
{
class SDL3Window
{
public:
    SDL3Window(const std::string& title, int width, int height);
    ~SDL3Window();

    SDL3Window(const SDL3Window&) = delete;
    SDL3Window& operator=(const SDL3Window&) = delete;
    SDL3Window(SDL3Window&&) = delete;
    SDL3Window& operator=(SDL3Window&&) = delete;

    [[nodiscard]] bool pollEvents();
    [[nodiscard]] void* getNativeHandle() const noexcept;

private:
    void* window{nullptr};
};
} // namespace engine::backends
