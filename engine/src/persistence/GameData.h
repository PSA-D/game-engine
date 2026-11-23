//
// Created by Jordy on 23/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_GAMEDATA_H
#define PROJECTSYSTEMARCHITECTURE_GAMEDATA_H

#pragma once

#include "runtime/DataNode.h"

#include <cstdint>

namespace engine::persistence
{
// De bestandsvorm van opgeslagen gegevens: een versienummer en een boom met de data.
// De betekenis van de boom hangt af van wat er opgeslagen wordt.
struct GameData
{
    std::uint32_t version{1};
    runtime::DataNode root;

    bool operator==(const GameData&) const = default;
};
} // namespace engine::persistence

#endif // PROJECTSYSTEMARCHITECTURE_GAMEDATA_H
