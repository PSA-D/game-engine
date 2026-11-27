#include "PersistenceSubsystem.h"

#include "platform/IPlatformPaths.h"
#include "core/ComponentRegistry.h"
#include "runtime/Level.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace engine::subsystems
{
namespace
{
constexpr std::string_view SAVES_FOLDER = "saves";
constexpr std::string_view LEVELS_FOLDER = "levels";
constexpr std::size_t MAX_NAME_LENGTH = 64;

// Namen die Windows als apparaat ziet, ook met een extensie (bijvoorbeeld "CON.json").
constexpr std::array<std::string_view, 22> RESERVED_NAMES = {
    "CON",  "PRN",  "AUX",  "NUL",  "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7",
    "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

auto toLowerAscii(char character) -> char
{
    if (character >= 'A' && character <= 'Z')
        return static_cast<char>(character - 'A' + 'a');
    return character;
}

auto equalsIgnoreCase(std::string_view left, std::string_view right) -> bool
{
    return left.size() == right.size() &&
           std::equal(left.begin(), left.end(), right.begin(),
                      [](char a, char b) { return toLowerAscii(a) == toLowerAscii(b); });
}

auto isAllowedCharacter(char character) -> bool
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') ||
           (character >= '0' && character <= '9') || character == '_' || character == '-' ||
           character == ' ';
}
} // namespace

PersistenceSubsystem::PersistenceSubsystem(std::unique_ptr<ISaveStrategy> saveStrategy,
                                           const platform::IPlatformPaths& platformPaths,
                                           ComponentRegistry& componentRegistry)
    : strategy(std::move(saveStrategy)), paths(platformPaths), serializer(componentRegistry)
{
    if (strategy == nullptr)
        throw std::invalid_argument("PersistenceSubsystem: de save-strategie is nullptr.");
}

bool PersistenceSubsystem::saveLevel(const std::string& name, const Level& level)
{
    lastError = PersistenceError::None;
    if (!isValidName(name))
    {
        lastError = PersistenceError::InvalidName;
        return false;
    }

    const auto text = strategy->serialize(serializer.toGameData(level));
    return writeFile(pathFor(paths.getUserDataPath(), LEVELS_FOLDER, name), text);
}

std::unique_ptr<Level> PersistenceSubsystem::loadLevel(const std::string& name)
{
    lastError = PersistenceError::None;
    if (!isValidName(name))
    {
        lastError = PersistenceError::InvalidName;
        return nullptr;
    }

    // Eerst de eigen map (door de speler of editor bewaard), dan de meegeleverde levels.
    auto file = pathFor(paths.getUserDataPath(), LEVELS_FOLDER, name);
    std::error_code ignored;
    if (!std::filesystem::exists(file, ignored))
        file = pathFor(paths.getAssetRoot(), LEVELS_FOLDER, name);

    const auto text = readAll(file);
    if (!text)
        return nullptr;

    const auto data = strategy->deserialize(*text);
    auto level = data ? serializer.fromGameData(*data) : nullptr;
    if (level == nullptr)
        lastError = PersistenceError::InvalidData;
    return level;
}

bool PersistenceSubsystem::saveGame(const std::string& slot, const GameData& data)
{
    lastError = PersistenceError::None;
    if (!isValidName(slot))
    {
        lastError = PersistenceError::InvalidName;
        return false;
    }

    return writeFile(pathFor(paths.getUserDataPath(), SAVES_FOLDER, slot),
                     strategy->serialize(data));
}

std::optional<GameData> PersistenceSubsystem::loadGame(const std::string& slot)
{
    lastError = PersistenceError::None;
    if (!isValidName(slot))
    {
        lastError = PersistenceError::InvalidName;
        return std::nullopt;
    }

    const auto text = readAll(pathFor(paths.getUserDataPath(), SAVES_FOLDER, slot));
    if (!text)
        return std::nullopt;

    auto data = strategy->deserialize(*text);
    if (!data || data->version == 0)
    {
        lastError = PersistenceError::InvalidData;
        return std::nullopt;
    }
    if (data->version > SAVE_GAME_VERSION)
    {
        lastError = PersistenceError::UnsupportedVersion;
        return std::nullopt;
    }
    return data;
}

bool PersistenceSubsystem::hasSave(const std::string& slot) const
{
    if (!isValidName(slot))
        return false;

    std::error_code ignored;
    return std::filesystem::is_regular_file(pathFor(paths.getUserDataPath(), SAVES_FOLDER, slot),
                                            ignored);
}

bool PersistenceSubsystem::deleteSave(const std::string& slot)
{
    lastError = PersistenceError::None;
    if (!isValidName(slot))
    {
        lastError = PersistenceError::InvalidName;
        return false;
    }

    std::error_code error;
    const bool removed =
        std::filesystem::remove(pathFor(paths.getUserDataPath(), SAVES_FOLDER, slot), error);
    if (error)
    {
        lastError = PersistenceError::IoError;
        return false;
    }
    if (!removed)
    {
        lastError = PersistenceError::NotFound;
        return false;
    }
    return true;
}

std::vector<std::string> PersistenceSubsystem::listSaves() const
{
    std::vector<std::string> slots;
    const std::string extension = strategy->fileExtension();
    const auto folder = paths.getUserDataPath() / std::filesystem::path(SAVES_FOLDER);

    std::error_code error;
    for (std::filesystem::directory_iterator it(folder, error), end; !error && it != end;
         it.increment(error))
    {
        std::error_code statusError;
        if (!it->is_regular_file(statusError) || statusError)
            continue;
        if (it->path().extension().string() != extension)
            continue;
        // bijvoorbeeld een tijdelijk bestand

        auto slot = it->path().stem().string();
        if (isValidName(slot))
            slots.push_back(std::move(slot));
    }

    std::sort(slots.begin(), slots.end());
    return slots;
}

PersistenceError PersistenceSubsystem::getLastError() const
{
    return lastError;
}

bool PersistenceSubsystem::isValidName(std::string_view name)
{
    if (name.empty() || name.size() > MAX_NAME_LENGTH)
        return false;
    if (name.front() == ' ' || name.back() == ' ')
        return false;
    // Windows haalt spaties aan het eind van een bestandsnaam weg
    if (!std::all_of(name.begin(), name.end(), isAllowedCharacter))
        return false;

    return std::none_of(RESERVED_NAMES.begin(), RESERVED_NAMES.end(),
                        [name](std::string_view reserved)
                        { return equalsIgnoreCase(name, reserved); });
}

std::filesystem::path PersistenceSubsystem::pathFor(const std::filesystem::path& root,
                                                    std::string_view folder,
                                                    const std::string& name) const
{
    return root / std::filesystem::path(folder) / (name + strategy->fileExtension());
}

bool PersistenceSubsystem::writeFile(const std::filesystem::path& file, const std::string& text)
{
    std::error_code error;
    std::filesystem::create_directories(file.parent_path(), error);
    if (error)
    {
        lastError = PersistenceError::IoError;
        return false;
    }

    // Eerst naar een tijdelijk bestand, zodat een bestaande save heel blijft als dit mislukt.
    auto temporary = file;
    temporary += ".tmp";
    std::error_code ignored;
    {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output.is_open())
        {
            lastError = PersistenceError::IoError;
            return false;
        }
        output << text;
        output.flush();
        if (!output)
        {
            output.close();
            std::filesystem::remove(temporary, ignored);
            lastError = PersistenceError::IoError;
            return false;
        }
    }

    std::filesystem::rename(temporary, file, error);
    if (error)
    {
        // Niet elk platform vervangt een bestaand bestand: haal het oude weg en probeer opnieuw.
        std::filesystem::remove(file, ignored);
        error.clear();
        std::filesystem::rename(temporary, file, error);
    }
    if (error)
    {
        std::filesystem::remove(temporary, ignored);
        lastError = PersistenceError::IoError;
        return false;
    }
    return true;
}

std::optional<std::string> PersistenceSubsystem::readAll(const std::filesystem::path& file)
{
    std::error_code error;
    if (!std::filesystem::exists(file, error))
    {
        lastError = error ? PersistenceError::IoError : PersistenceError::NotFound;
        return std::nullopt;
    }

    std::ifstream input(file, std::ios::binary);
    if (!input.is_open())
    {
        lastError = PersistenceError::IoError;
        return std::nullopt;
    }

    std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    if (input.bad())
    {
        lastError = PersistenceError::IoError;
        return std::nullopt;
    }
    return text;
}
} // namespace engine::subsystems