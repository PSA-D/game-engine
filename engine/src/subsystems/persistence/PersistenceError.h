#pragma once

#include <cstdint>
#include <string_view>

namespace engine::subsystems
{
// De reden waarom de laatste opslag- of laadactie mislukte.
enum class PersistenceError : std::uint8_t
{
    None,               // gelukt
    NotAvailable,       // de engine draait niet, dus er is geen opslag
    InvalidName,        // de naam van het slot of level is niet toegestaan
    NotFound,           // er is geen bestand met die naam
    IoError,            // het bestand kon niet gelezen of geschreven worden
    InvalidData,        // het bestand is beschadigd of heeft de verkeerde inhoud
    UnsupportedVersion, // het bestand komt van een nieuwere versie van het spel
};

// Een korte tekst die in een log of aan de speler getoond kan worden.
[[nodiscard]] constexpr std::string_view describeError(PersistenceError error)
{
    switch (error)
    {
    case PersistenceError::None:
        return "Gelukt.";
    case PersistenceError::NotAvailable:
        return "Opslaan en laden is niet beschikbaar: de engine draait niet.";
    case PersistenceError::InvalidName:
        return "Deze naam is niet toegestaan. Gebruik letters, cijfers, spaties, - en _.";
    case PersistenceError::NotFound:
        return "Het bestand bestaat niet.";
    case PersistenceError::IoError:
        return "Het bestand kon niet gelezen of geschreven worden.";
    case PersistenceError::InvalidData:
        return "Het bestand is beschadigd of heeft een ongeldige inhoud.";
    case PersistenceError::UnsupportedVersion:
        return "Het bestand komt van een nieuwere versie van het spel.";
    }
    return "Onbekende fout.";
}
} // namespace engine::subsystems
