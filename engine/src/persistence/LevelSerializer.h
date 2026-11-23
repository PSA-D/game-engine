//
// Created by Jordy on 23/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_LEVELSERIALIZER_H
#define PROJECTSYSTEMARCHITECTURE_LEVELSERIALIZER_H

#pragma once

#include <cstdint>
#include <memory>

#include "GameData.h"

namespace engine::runtime
{
class Level;
class ComponentRegistry;
}

namespace engine::persistence
{
class LevelSerializer
{
public:
    explicit LevelSerializer(runtime::ComponentRegistry& registry);

    [[nodiscard]]
    GameData toGameData(const runtime::Level& level) const;

    [[nodiscard]]
    std::unique_ptr<runtime::Level>
    fromGameData(const GameData& data) const;

private:
    runtime::ComponentRegistry& registry;

    static constexpr std::uint32_t CurrentVersion = 1;
};
}

#endif // PROJECTSYSTEMARCHITECTURE_LEVELSERIALIZER_H
