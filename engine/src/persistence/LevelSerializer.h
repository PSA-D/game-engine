#pragma once

#include "GameData.h"
#include "runtime/Level.h"

#include <cstdint>
#include <memory>

namespace engine::runtime
{
class Level;
class ComponentRegistry;
}

namespace engine::subsystems
{
class LevelSerializer
{
public:
    explicit LevelSerializer(runtime::ComponentRegistry& registry);

    [[nodiscard]]
    GameData toGameData(const Level& level) const;

    [[nodiscard]]
    std::unique_ptr<runtime::Level>
    fromGameData(const GameData& data) const;

private:
    runtime::ComponentRegistry& registry;

    static constexpr std::uint32_t CurrentVersion = 1;
};
} // engine::subsystems
