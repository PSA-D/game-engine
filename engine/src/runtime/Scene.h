#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
class Level;
class Region;

// Een scene: een verzameling Levels, waarvan er één actief is.
// De actieve Region van het actieve Level is het deel van de wereld waar het spel zich nu afspeelt.
//
// Wat "de actieve inhoud" betekent: createGameObject() en alle find-functies werken op de actieve Region van het
// actieve Level. Objecten in andere Regions of Levels worden niet gevonden, ook al bestaan ze nog.
//
// - Een nieuw Level is niet vanzelf actief. Activeer het met activateLevel().
// - Een Scene bezit zijn Levels. Hij kan niet gekopieerd of verplaatst worden.
class Scene
{
public:
    Scene(std::uint64_t sceneId, std::string sceneName);
    ~Scene();
    Scene(const Scene&) = delete;
    auto operator=(const Scene&) -> Scene& = delete;
    Scene(Scene&&) = delete;
    auto operator=(Scene&&) -> Scene& = delete;

    [[nodiscard]] std::uint64_t getId() const;
    [[nodiscard]] const std::string& getName() const;

    // ---- Levels ----

    Level& createLevel(std::string_view levelName);

    // Geeft false (en verandert niets) als het Level niet bij deze Scene hoort.
    bool activateLevel(Level& level);

    // nullptr als er geen actief Level is.
    [[nodiscard]] Level* getActiveLevel();
    [[nodiscard]] const Level* getActiveLevel() const;

    // In volgorde van aanmaken.
    [[nodiscard]] std::vector<Level*> getLevels();
    [[nodiscard]] std::vector<const Level*> getLevels() const;

    // Vervangt het actieve Level door het gegeven Level, dat meteen actief wordt.
    // Het oude Level wordt vernietigd, met al zijn objecten.
    // Was er geen actief Level, dan wordt het nieuwe gewoon toegevoegd.
    // Gooit std::invalid_argument bij nullptr.
    // Niet aanroepen tijdens een Update: verwijzingen naar het oude Level zijn daarna ongeldig.
    void replaceLevel(std::unique_ptr<Level> level);

    // ---- GameObjects (in de actieve Region van het actieve Level) ----

    // TODO: Begint hier
    // // Gooit std::logic_error als er geen actief Level met een actieve Region is.
    // GameObject& createGameObject(std::string_view objectName);
    //
    // // nullptr als er niets gevonden wordt.
    // [[nodiscard]] GameObject* findGameObject(std::uint64_t id);
    // // Het eerste object (in volgorde van aanmaken) met deze naam.
    // [[nodiscard]] GameObject* find(std::string_view objectName);
    // [[nodiscard]] std::vector<GameObject*> findWithTag(std::string_view tag);
    //
    // // Alle componenten van type T op de objecten in de actieve Region.
    // template <class T> [[nodiscard]] std::vector<T*> findComponents()
    // {
    //     std::vector<T*> result;
    //     for (auto* gameObject : collectActiveGameObjects())
    //     {
    //         const auto components = gameObject->getComponents<T>();
    //         result.insert(result.end(), components.begin(), components.end());
    //     }
    //     return result;
    // }
    //
    // // Ruimt de aangevraagde vernietigingen op, in alle Regions van alle Levels.
    // // Aan te roepen aan het eind van een frame.
    // void processPendingDestruction();
    // // updateBehaviours() volgt in een aparte taak, samen met BehaviourScript.
    // TODO: Uncomment de bovenstaande code zodra GameObject beschikbaar is

private:
    [[nodiscard]] Region* getActiveRegion() const;
    // [[nodiscard]] std::vector<GameObject*> collectActiveGameObjects();
    // TODO: Uncomment de bovenstaande code zodra GameObject beschikbaar is

    std::uint64_t id;
    std::string name;
    std::vector<std::unique_ptr<Level>> levels;
    Level* activeLevel{nullptr};
};
} // namespace engine
