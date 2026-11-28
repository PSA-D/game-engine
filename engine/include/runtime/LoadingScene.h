#pragma once

#include "Scene.h"

#include <cstdint>
#include <string>

namespace engine
{
    class LoadingScene : public Scene
    {
    public:
        LoadingScene(std::uint64_t id, std::string name)
            : Scene(id, std::move(name))
        {
        }

        LoadingScene(const LoadingScene&) = delete;
        auto operator=(const LoadingScene&) -> LoadingScene& = delete;
        LoadingScene(LoadingScene&&) = delete;
        auto operator=(LoadingScene&&) noexcept -> LoadingScene& = delete;

        [[nodiscard]] float getProgress() const noexcept
        {
            return progress;
        };

    private:
        float progress{0.0f}; // 0.0-1.0
    };
} // namespace engine
