#pragma once

#include "persistence/GameData.h"
#include "runtime/DataNode.h"

#include <utility>

namespace engine::subsystems
{
    // Gedeelde testdata voor de unit- en integratietests van persistence.
    // Bouwt een voorbeeld met elk datatype, ook genest.
    inline auto makeSampleData() -> GameData
    {
        using engine::DataNode;

        auto player = DataNode::makeObject();
        player.asObject().emplace("name", DataNode("Hero"));
        player.asObject().emplace("health", DataNode(75));
        player.asObject().emplace("speed", DataNode(3.5));
        player.asObject().emplace("alive", DataNode(true));
        player.asObject().emplace("nickname", DataNode());

        auto positions = DataNode::makeArray();
        positions.asArray().push_back(DataNode(1.5));
        positions.asArray().push_back(DataNode(-2.25));

        auto root = DataNode::makeObject();
        root.asObject().emplace("player", std::move(player));
        root.asObject().emplace("positions", std::move(positions));

        GameData data;
        data.version = 3;
        data.root = std::move(root);
        return data;
    }
} // namespace engine::subsystems
