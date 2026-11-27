// Laag: UNIT. Geen bestanden: alleen de regels voor namen, de foutteksten en het aanmaken.
// Wat er met echte bestanden gebeurt staat in PersistenceSubsystemIntegrationTest.cpp.

#include "persistence/JsonSaveStrategy.h"
#include "persistence/PersistenceSubsystem.h"
#include "core/ComponentRegistry.h"
#include "PersistenceTestData.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <array>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>

namespace engine::subsystems
{
TEST_CASE("Toegestane namen voor slots en levels", "[unit][persistence]")
{
    const auto name = GENERATE(as<std::string>{}, "slot1", "Save 1", "level-2_final", "A",
                               "Boss Fight 3");

    CHECK(PersistenceSubsystem::isValidName(name));
}

TEST_CASE("De lengte van een naam is begrensd", "[unit][persistence]")
{
    CHECK(PersistenceSubsystem::isValidName(std::string(64, 'a')));
    CHECK_FALSE(PersistenceSubsystem::isValidName(std::string(65, 'a')));
}

TEST_CASE("Niet-toegestane namen worden geweigerd", "[unit][persistence]")
{
    // Alles wat uit de map kan ontsnappen, een bestandsextensie of een apparaatnaam lijkt.
    const auto name = GENERATE(as<std::string>{}, "", "../evil", "a/b", "a\\b", "x.json", "naam.",
                               "c:naam", " begin", "eind ", "con", "NUL", "Com1", "lpt9",
                               "caf\xC3\xA9", "tab\tnaam", "ster*", "vraag?");

    CHECK_FALSE(PersistenceSubsystem::isValidName(name));
}

TEST_CASE("Elke fout heeft een eigen, niet-lege tekst", "[unit][persistence]")
{
    const std::array errors{PersistenceError::None,         PersistenceError::NotAvailable,
                            PersistenceError::InvalidName,  PersistenceError::NotFound,
                            PersistenceError::IoError,      PersistenceError::InvalidData,
                            PersistenceError::UnsupportedVersion};
    std::set<std::string_view> texts;

    for (const auto error : errors)
    {
        const auto text = describeError(error);
        CHECK_FALSE(text.empty());
        texts.insert(text);
    }

    CHECK(texts.size() == errors.size());
}

TEST_CASE("Een lege strategie wordt geweigerd", "[unit][persistence]")
{
    const NoDiskPaths paths;
    ComponentRegistry registry;

    CHECK_THROWS_AS(PersistenceSubsystem(nullptr, paths, registry), std::invalid_argument);
}

TEST_CASE("Een ongeldige naam geeft InvalidName en geen exception", "[unit][persistence]")
{
    const NoDiskPaths paths;
    ComponentRegistry registry;
    PersistenceSubsystem subsystem(std::make_unique<JsonSaveStrategy>(), paths, registry);
    CHECK(subsystem.getLastError() == PersistenceError::None);

    CHECK_FALSE(subsystem.saveGame("../evil", GameData{}));
    CHECK(subsystem.getLastError() == PersistenceError::InvalidName);

    CHECK_FALSE(subsystem.loadGame("a/b").has_value());
    CHECK(subsystem.getLastError() == PersistenceError::InvalidName);

    CHECK_FALSE(subsystem.hasSave(""));
}
} // namespace engine::subsystems