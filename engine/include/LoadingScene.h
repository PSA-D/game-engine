#pragma once

#include "Scene.h"

#include <cstdint>
#include <string>

namespace engine
{
class LoadingScene : public Scene
{
public:
    LoadingScene(std::uint64_t id, std::string name);
    ~LoadingScene() override;

    LoadingScene(const LoadingScene&) = delete;
    auto operator=(const LoadingScene&) -> LoadingScene& = delete;
    LoadingScene(LoadingScene&&) = delete;
    auto operator=(LoadingScene&&) noexcept -> LoadingScene& = delete;
};
} // namespace engine
