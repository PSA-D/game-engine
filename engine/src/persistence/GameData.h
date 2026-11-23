#pragma once

#include "runtime/DataNode.h"

#include <cstdint>

namespace engine::subsystem::PersistenceSubsystem
{
// De bestandsvorm van opgeslagen gegevens: een versienummer en een boom met de data.
// De betekenis van de boom hangt af van wat er opgeslagen wordt.
struct GameData
{
    std::uint32_t version{1};
    runtime::DataNode root;

    bool operator==(const GameData&) const = default;
};
} // namespace engine::subsystem::PersistenceSubsystem
