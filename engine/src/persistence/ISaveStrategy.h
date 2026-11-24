#pragma once

#include "GameData.h"

#include <optional>
#include <string>

namespace engine::subsystems
{
// Zet GameData om naar tekst en terug.
// opslagvorm (JSON, binair, ...) is een eigen implementatie van dit interface,
// zodat de rest van de engine niet weet hoe er opgeslagen wordt.
//
// Verantwoordelijkheid:
// - Een strategie zet alleen om. Bestanden lezen en schrijven doet PersistenceSubsystem.
//
// Contract:
// - serialize() lukt altijd en gooit geen exceptions.
// - deserialize() geeft std::nullopt bij ongeldige of beschadigde tekst, en gooit geen exceptions.
// - deserialize(serialize(data)) geeft weer dezelfde data terug.
//
// Eigenaarschap en levensduur:
// - De strategie heeft geen toestand en kan overal gedeeld worden.
// - PersistenceSubsystem bezit de strategie (unique_ptr).
class ISaveStrategy
{
public:
    ISaveStrategy() = default;
    virtual ~ISaveStrategy() = default;
    ISaveStrategy(const ISaveStrategy&) = delete;
    auto operator=(const ISaveStrategy&) -> ISaveStrategy& = delete;
    ISaveStrategy(ISaveStrategy&&) = delete;
    auto operator=(ISaveStrategy&&) -> ISaveStrategy& = delete;

    [[nodiscard]] virtual std::string serialize(const GameData& data) const = 0;
    [[nodiscard]] virtual std::optional<GameData> deserialize(const std::string& text) const = 0;

    // Bestandsextensie van deze opslagvorm, inclusief punt (bijvoorbeeld ".json").
    [[nodiscard]] virtual std::string fileExtension() const = 0;
};
} // namespace engine::subsystems

