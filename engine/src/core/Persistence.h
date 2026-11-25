#pragma once

#include "runtime/DataNode.h"
#include "runtime/Level.h"
#include "persistence/PersistenceError.h"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
namespace subsystems
{
class PersistenceSubsystem;
}
namespace systems
{
class PersistenceSubsystem;
} // namespace systems

class EngineImpl;
class PersistenceTestAccess;

// Voor game-code die iets wil weten over een mislukte actie.
using PersistenceError = subsystems::PersistenceError;

// Opslaan en laden voor game-ontwikkelaars.
// Dit is de enige ingang: game-code hoeft nooit met een subsysteem te praten.
//
// Savegames: de ontwikkelaar bouwt zelf een DataNode met wat bewaard moet worden
// (score, inventaris, voortgang, ...) en geeft die een slotnaam.
//
//   auto save = DataNode::makeObject();
//   save.set("score", DataNode(120));
//   Persistence::saveGame("slot1", save);
//   if (auto loaded = Persistence::loadGame("slot1"))
//       score = loaded->get("score")->tryInt().value_or(0);
//
// Fouten:
// - Een mislukte aanroep geeft false, std::nullopt of nullptr terug en gooit geen exception.
// - getLastError() zegt waarom, describeError() geeft er een tekst bij.
// - Draait de engine niet, dan mislukt elke aanroep met PersistenceError::NotAvailable.
//
// Namen van slots en levels: letters, cijfers, spaties (niet aan de randen), - en _.
class Persistence
{
public:
    Persistence() = delete;

    // ---- Savegames ----
    static bool saveGame(const std::string& slot, const DataNode& data);
    [[nodiscard]] static std::optional<DataNode> loadGame(const std::string& slot);
    [[nodiscard]] static bool hasSave(const std::string& slot);
    static bool deleteSave(const std::string& slot);
    // Alle slots, alfabetisch. Handig voor een laadmenu.
    [[nodiscard]] static std::vector<std::string> listSaves();

    // ---- Levels (bijvoorbeeld voor een editor of een eigen levelkeuze) ----
    static bool saveLevel(const std::string& name, const Level& level);
    [[nodiscard]] static std::unique_ptr<Level> loadLevel(const std::string& name);

    // ---- Fouten ----
    [[nodiscard]] static PersistenceError getLastError();
    [[nodiscard]] static std::string_view describeError(PersistenceError error);

private:
    friend class EngineImpl;
    // zet de koppeling bij het starten en stoppen
    friend class PersistenceTestAccess;
    // alleen voor tests

    static void setSystem(systems::PersistenceSubsystem* persistenceSubsystem);

    static subsystems::PersistenceSubsystem* subsystem;
};
} // namespace engine