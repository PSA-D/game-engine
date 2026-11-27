#include "Persistence.h"

#include "core/Persistence.h"
#include "persistence/GameData.h"
#include "persistence/PersistenceSubsystem.h"

#include <utility>

namespace engine
{
subsystems::PersistenceSubsystem* Persistence::subsystem = nullptr;

void Persistence::setSystem(subsystems::PersistenceSubsystem* persistenceSubsystem)
{
    subsystem = persistenceSubsystem;
}

bool Persistence::saveGame(const std::string& slot, const DataNode& data)
{
    if (subsystem == nullptr)
        return false;

    // De ontwikkelaar geeft alleen zijn eigen data. De versie voegt de engine toe.
    subsystems::GameData gameData;
    gameData.version = subsystems::PersistenceSubsystem::SAVE_GAME_VERSION;
    gameData.root = data;
    return subsystem->saveGame(slot, gameData);
}

std::optional<DataNode> Persistence::loadGame(const std::string& slot)
{
    if (subsystem == nullptr)
        return std::nullopt;

    auto gameData = subsystem->loadGame(slot);
    if (!gameData)
        return std::nullopt;
    return std::move(gameData->root);
}

bool Persistence::hasSave(const std::string& slot)
{
    return subsystem != nullptr && subsystem->hasSave(slot);
}

bool Persistence::deleteSave(const std::string& slot)
{
    return subsystem != nullptr && subsystem->deleteSave(slot);
}

std::vector<std::string> Persistence::listSaves()
{
    if (subsystem == nullptr)
        return {};
    return subsystem->listSaves();
}

bool Persistence::saveLevel(const std::string& name, const Level& level)
{
    return subsystem != nullptr && subsystem->saveLevel(name, level);
}

std::unique_ptr<Level> Persistence::loadLevel(const std::string& name)
{
    if (subsystem == nullptr)
        return nullptr;
    return subsystem->loadLevel(name);
}

PersistenceError Persistence::getLastError()
{
    return subsystem == nullptr ? PersistenceError::NotAvailable : subsystem->getLastError();
}

std::string_view Persistence::describeError(PersistenceError error)
{
    return subsystems::describeError(error);
}
} // namespace engine