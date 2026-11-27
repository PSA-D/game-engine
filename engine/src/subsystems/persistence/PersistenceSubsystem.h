#pragma once

#include "GameData.h"
#include "ISaveStrategy.h"
#include "LevelSerializer.h"
#include "PersistenceError.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
class ComponentRegistry;
class Level;
namespace platform
{
class IPlatformPaths;
} // namespace platform
} // namespace engine

namespace engine::subsystems
{
// Bewaart en laadt savegames en levels als bestanden. Hoe de data eruitziet (JSON of iets anders) bepaalt de ISaveStrategy.
// Waar de bestanden staan, bepaalt IPlatformPaths.
//
// Bestanden (extensie komt van de strategie, hier ".json"):
//   <gebruikersmap>/saves/<slot>.json    savegames
//   <gebruikersmap>/levels/<naam>.json   levels die de speler of de editor bewaarde
//   <assetmap>/levels/<naam>.json        meegeleverde levels (alleen lezen)
// loadLevel() zoekt eerst in de gebruikersmap en daarna in de assetmap.
//
// Foutafhandeling:
// - Er worden geen exceptions gegooid door slechte invoer of bestanden. Een mislukte actie geeft false, std::nullopt of nullptr terug.
// - getLastError() zegt waarom. Elke schrijf- of laadactie zet hem opnieuw (None bij succes).
// - Opslaan schrijft eerst naar een tijdelijk bestand en vervangt daarna pas het echte bestand.
//   Een crash tijdens het opslaan beschadigt dus geen bestaande save.
//
// Namen van slots en levels worden bestandsnamen. Daarom zijn alleen letters, cijfers, spaties (niet vooraan of achteraan), - en _ toegestaan.
// Zo kan een naam nooit uit de map ontsnappen (bijvoorbeeld "../x").
//
// Eigenaarschap en levensduur:
// - Het subsysteem bezit de strategie. Paden en registry moeten langer leven.
class PersistenceSubsystem
{
public:
    // Versie van het savegame-formaat dat deze engine schrijft en leest.
    static constexpr std::uint32_t SAVE_GAME_VERSION = 1;

    // Gooit std::invalid_argument als de strategie nullptr is.
    PersistenceSubsystem(std::unique_ptr<ISaveStrategy> saveStrategy,
                         const platform::IPlatformPaths& platformPaths,
                         ComponentRegistry& componentRegistry);
    ~PersistenceSubsystem() = default;
    PersistenceSubsystem(const PersistenceSubsystem&) = delete;
    auto operator=(const PersistenceSubsystem&) -> PersistenceSubsystem& = delete;
    PersistenceSubsystem(PersistenceSubsystem&&) = delete;
    auto operator=(PersistenceSubsystem&&) -> PersistenceSubsystem& = delete;

    // ---- Levels ----
    bool saveLevel(const std::string& name, const Level& level);
    // nullptr bij een fout, zie getLastError().
    [[nodiscard]] std::unique_ptr<Level> loadLevel(const std::string& name);

    // ---- Savegames ----
    bool saveGame(const std::string& slot, const GameData& data);
    // std::nullopt bij een fout, zie getLastError().
    [[nodiscard]] std::optional<GameData> loadGame(const std::string& slot);
    [[nodiscard]] bool hasSave(const std::string& slot) const;
    // false als het slot niet bestaat (NotFound) of niet verwijderd kon worden (IoError).
    bool deleteSave(const std::string& slot);
    // Alle slots die er zijn, alfabetisch. Handig voor een laadmenu.
    [[nodiscard]] std::vector<std::string> listSaves() const;

    [[nodiscard]] PersistenceError getLastError() const;

    // Mag deze naam gebruikt worden als slot of levelnaam?
    [[nodiscard]] static bool isValidName(std::string_view name);

private:
    [[nodiscard]] std::filesystem::path pathFor(const std::filesystem::path& root,
                                                std::string_view folder,
                                                const std::string& name) const;
    // Zetten lastError bij een mislukking.
    bool writeFile(const std::filesystem::path& file, const std::string& text);
    [[nodiscard]] std::optional<std::string> readAll(const std::filesystem::path& file);

    std::unique_ptr<ISaveStrategy> strategy;
    const platform::IPlatformPaths& paths;
    LevelSerializer serializer;
    PersistenceError lastError{PersistenceError::None};
};
} // namespace engine::subsystems