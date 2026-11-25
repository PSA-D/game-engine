#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
    class Region;

    // Een level van het spel: een verzameling Regions, waarvan er één actief is.
    //
    // - Een nieuwe Region is niet vanzelf actief. Activeer hem met activateRegion().
    // - Een Level bezit zijn Regions. Hij kan niet gekopieerd of verplaatst worden.
    class Level
    {
    public:
        explicit Level(std::string levelName);
        ~Level();
        Level(const Level&) = delete;
        auto operator=(const Level&) -> Level& = delete;
        Level(Level&&) = delete;
        auto operator=(Level&&) -> Level& = delete;

        [[nodiscard]] const std::string& getName() const;

        // In volgorde van aanmaken. De pointers blijven geldig zolang het Level bestaat.
        [[nodiscard]] std::vector<Region*> getRegions();
        [[nodiscard]] std::vector<const Region*> getRegions() const;

        // Namen hoeven niet uniek te zijn.
        Region& createRegion(std::string_view regionName);

        // Geeft false (en verandert niets) als de Region niet bij dit Level hoort.
        bool activateRegion(Region& region);

        // nullptr als er geen actieve Region is.
        [[nodiscard]] Region* getActiveRegion();
        [[nodiscard]] const Region* getActiveRegion() const;

    private:
        std::string name;
        std::vector<std::unique_ptr<Region>> regions;
        Region* activeRegion{nullptr};
    };
} // namespace engine
