#include "Level.h"

#include "Region.h"

#include <algorithm>
#include <utility>

namespace engine
{
    Level::Level(std::string levelName)
        : name(std::move(levelName))
    {
    }

    Level::~Level() = default;

    const std::string& Level::getName() const
    {
        return name;
    }

    std::vector<Region*> Level::getRegions()
    {
        std::vector<Region*> result;
        result.reserve(regions.size());
        for (auto& region : regions)
            result.push_back(region.get());
        return result;
    }

    std::vector<const Region*> Level::getRegions() const
    {
        std::vector<const Region*> result;
        result.reserve(regions.size());
        for (const auto& region : regions)
            result.push_back(region.get());
        return result;
    }

    Region& Level::createRegion(std::string_view regionName)
    {
        regions.push_back(std::make_unique<Region>(std::string(regionName)));
        return *regions.back();
    }

    bool Level::activateRegion(Region& region)
    {
        const bool owned = std::any_of(
            regions.begin(), regions.end(), [&region](const auto& candidate) { return candidate.get() == &region; });
        if (!owned)
            return false;

        activeRegion = &region;
        return true;
    }

    Region* Level::getActiveRegion()
    {
        return activeRegion;
    }

    const Region* Level::getActiveRegion() const
    {
        return activeRegion;
    }
} // namespace engine
