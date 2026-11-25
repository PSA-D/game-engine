#pragma once

#include "GameData.h"
#include "persistence/GameData.h"
#include "runtime/Level.h"

#include <cstdint>
#include <memory>

namespace engine::runtime
{
    class ComponentRegistry;
    class Level;
} // namespace engine::runtime

namespace engine::subsystems
{
    // Zet een Level om naar GameData en terug. Het resultaat kan met een ISaveStrategy
    // (bijvoorbeeld JsonSaveStrategy) naar een bestand.
    //
    // Bestandsvorm van GameData::root:
    //   { "name": "Level 1",
    //     "activeRegion": 1,                      (optioneel, index in "regions")
    //     "regions": [
    //       { "name": "Start",
    //         "gameObjects": [
    //           { "id": 7, "name": "Player", "tag": "Player", "parent": null,
    //             "transform": { "position": [x, y], "rotation": r, "scale": [x, y] },
    //             "components": [ <door ComponentRegistry gemaakt> ] } ] } ] }
    //
    // - "id" en "parent" gelden alleen binnen het bestand om de hiërarchie te leggen. Bij het
    //   inlezen krijgt elk GameObject een nieuw id van zijn Region.
    // - De transform is lokaal (ten opzichte van de ouder).
    // - Een component dat niet in de ComponentRegistry staat, kan niet bewaard worden en wordt
    //   overgeslagen. De rest van het level wordt wel bewaard.
    //
    // Contract van fromGameData():
    // - Geeft nullptr bij elke fout: te nieuwe of onbekende versie, ontbrekende of verkeerde
    //   velden, dubbele ids, een onbekende ouder of een cyclus, een onbekend componenttype of
    //   ongeldige componentdata. Een half ingelezen level wordt nooit teruggegeven.
    // - Gooit geen exceptions door slechte invoer.
    // - Velden die niet verplicht zijn ("tag", "parent", "transform", "components",
    //   "activeRegion") mogen ontbreken. Onbekende extra velden worden genegeerd.
    //
    // Eigenaarschap en levensduur:
    // - De serializer bezit de registry niet, die moet langer leven dan de serializer.
    // - Het teruggegeven Level is van de aanroeper (unique_ptr).
    class LevelSerializer
    {
    public:
        explicit LevelSerializer(runtime::ComponentRegistry& componentRegistry);

        [[nodiscard]] GameData toGameData(const runtime::Level& level) const;
        [[nodiscard]] std::unique_ptr<runtime::Level> fromGameData(const GameData& data) const;

    private:
        runtime::ComponentRegistry& registry;

        static constexpr std::uint32_t CURRENT_VERSION = 1;
    };
} // namespace engine::subsystems

