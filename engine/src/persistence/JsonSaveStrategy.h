//
// Created by Jordy on 23/11/2026.
//

#ifndef PROJECTSYSTEMARCHITECTURE_JSONSAVESTRATEGY_H
#define PROJECTSYSTEMARCHITECTURE_JSONSAVESTRATEGY_H

#pragma once

#include "ISaveStrategy.h"

namespace engine::persistence
{
// Slaat GameData op als leesbare JSON, met nlohmann::json.
//
// Bestandsvorm: {"root": <de boom>, "version": <getal>}
// - Gehele getallen blijven gehele getallen, kommagetallen blijven kommagetallen.
// - Een niet-eindig kommagetal (NaN of oneindig) kan JSON niet bevatten en wordt null.
// - Tekst die geen geldige UTF-8 is, krijgt vervangtekens in plaats van een fout.
class JsonSaveStrategy final : public ISaveStrategy
{
public:
    [[nodiscard]] std::string serialize(const GameData& data) const override;
    [[nodiscard]] std::optional<GameData> deserialize(const std::string& text) const override;
    [[nodiscard]] std::string fileExtension() const override;
};
} // namespace engine::persistence

#endif // PROJECTSYSTEMARCHITECTURE_JSONSAVESTRATEGY_H
