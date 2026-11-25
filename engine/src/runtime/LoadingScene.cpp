#include "LoadingScene.h"

namespace engine
{
LoadingScene::LoadingScene(std::uint64_t id, std::string sceneName)
    : Scene(id, std::move(sceneName))
{
}

LoadingScene::~LoadingScene() = default;
} // namespace engine
