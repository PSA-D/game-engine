//
// Created by Jordy on 23/11/2026.
//

#include "LevelSerializer.h"

#include <stdexcept>

namespace engine
{
    class Level;
}
namespace engine::subsystems
{

LevelSerializer::LevelSerializer(
    engine::ComponentRegistry& registry)
    : registry(registry)
{
}

GameData LevelSerializer::toGameData(
    const Level& level) const
{
    // TODO:
    // Serialize:
    // - level name
    // - regions
    // - game objects
    // - components

    GameData data;
    data.version = CurrentVersion;

    auto root = DataNode::makeObject();

    // root["name"] = level.getName();
    // root["regions"] = ...

    data.root = std::move(root);

    return data;
}

std::unique_ptr<runtime::Level>
LevelSerializer::fromGameData(
    const GameData& data) const
{
    if (!data.root.isObject())
    {
        return nullptr;
    }

    // TODO:
    // Create level
    // Reconstruct regions
    // Reconstruct game objects
    // Reconstruct components

    return nullptr;
}

}
