#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace engine
{
class GameObject;

// Een afgebakend deel van een Level waarin GameObjects leven, bijvoorbeeld een kamer of gebied.
// Alleen de actieve Region van een Level doet mee aan het spel.
//
// Eigenaarschap en levensduur:
// - De Region bezit al zijn GameObjects, ook de kinderen in een hiërarchie. De lijst is plat, de ouder-kindrelatie staat in de GameObjects zelf.
// - Een Region wordt bezeten door een Level. Hij kan niet gekopieerd of verplaatst worden, want GameObjects verwijzen naar hem.
//
// Vernietigen is uitgesteld: destroy() op een GameObject vraagt het alleen aan.
// Het echte opruimen gebeurt in processPendingDestruction(), aan het eind van een frame.
// Zo verdwijnt er nooit een object halverwege een frame.
class Region
{
public:
    explicit Region(std::string regionName);
    ~Region();
    Region(const Region&) = delete;
    auto operator=(const Region&) -> Region& = delete;
    Region(Region&&) = delete;
    auto operator=(Region&&) -> Region& = delete;

    // Maakt een GameObject met een nieuw, uniek id (nooit 0).
    // De referentie blijft geldig tot het object vernietigd wordt.
    GameObject& createGameObject(std::string_view objectName);

    // Vraagt aan om een object (en al zijn kinderen) te vernietigen.
    // Meerdere keren aanvragen mag. Een object van een andere Region wordt genegeerd.
    void requestDestroy(GameObject& gameObject);

    // Voert alle aangevraagde vernietigingen uit.
    // Een verzoek dat tijdens het opruimen ontstaat, wordt in dezelfde aanroep afgehandeld.
    void processPendingDestruction();

    [[nodiscard]] const std::string& getName() const;

    // In volgorde van aanmaken. Objecten die op vernietiging wachten staan er nog in(zie GameObject::isPendingDestroy).
    [[nodiscard]] std::vector<GameObject*> getGameObjects();
    [[nodiscard]] std::vector<const GameObject*> getGameObjects() const;

private:
    // Vernietigt eerst alle kinderen, daarna het object zelf.
    void destroyRecursive(GameObject& gameObject);

    std::string name;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::uint64_t> pendingDestruction;
    // ids, geen pointers: een object kan al weg zijn
};
} // namespace engine