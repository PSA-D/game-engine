// Laag: UNIT. Alles in het geheugen. Region wordt getest met echte GameObjects.

#include "src/runtime/Region.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace engine
{
// namespace
// {
// auto namesOf(const Region& region) -> std::vector<std::string>
// {
//     std::vector<std::string> names;
//     for (const auto* gameObject : region.getGameObjects())
//         names.push_back(gameObject->getName());
//     return names;
// }
// } // namespace
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is

TEST_CASE("Een nieuwe Region heeft een naam", "[unit][runtime]")
{
    const Region region("Start");

    CHECK(region.getName() == "Start");
}

// TEST_CASE("createGameObject maakt een object in deze Region", "[unit][runtime]")
// {
//     Region region("Start");
//
//     auto& player = region.createGameObject("Player");
//
//     CHECK(player.getName() == "Player");
//     CHECK(&player.getRegion() == &region);
//     REQUIRE(region.getGameObjects().size() == 1);
//     CHECK(region.getGameObjects().front() == &player);
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("GameObjects staan in volgorde van aanmaken", "[unit][runtime]")
// {
//     Region region("Start");
//     region.createGameObject("A");
//     region.createGameObject("B");
//     region.createGameObject("C");
//
//     CHECK(namesOf(region) == std::vector<std::string>{"A", "B", "C"});
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Ids zijn uniek en nooit 0, ook over Regions heen", "[unit][runtime]")
// {
//     Region first("Een");
//     Region second("Twee");
//     std::set<std::uint64_t> ids;
//
//     for (int index = 0; index < 100; ++index)
//     {
//         ids.insert(first.createGameObject("x").getId());
//         ids.insert(second.createGameObject("y").getId());
//     }
//
//     CHECK(ids.size() == 200);
//     CHECK_FALSE(ids.contains(0));
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een referentie blijft geldig als er meer objecten bijkomen", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& first = region.createGameObject("Eerste");
//
//     for (int index = 0; index < 1000; ++index)
//         region.createGameObject("Vulling");
//
//     CHECK(first.getName() == "Eerste");
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een object verdwijnt pas bij processPendingDestruction", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& doomed = region.createGameObject("A");
//     region.createGameObject("B");
//
//     doomed.destroy();
//
//     CHECK(doomed.isPendingDestroy());
//     CHECK(region.getGameObjects().size() == 2); // nog aanwezig tot het opruimen
//
//     region.processPendingDestruction();
//
//     CHECK(namesOf(region) == std::vector<std::string>{"B"});
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("requestDestroy werkt ook rechtstreeks", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& doomed = region.createGameObject("A");
//
//     region.requestDestroy(doomed);
//     region.processPendingDestruction();
//
//     CHECK(region.getGameObjects().empty());
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Meerdere keren aanvragen vernietigt het object maar één keer", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& doomed = region.createGameObject("A");
//     region.createGameObject("B");
//
//     doomed.destroy();
//     doomed.destroy();
//     region.requestDestroy(doomed);
//     region.processPendingDestruction();
//
//     CHECK(namesOf(region) == std::vector<std::string>{"B"});
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("De volgorde van de overige objecten blijft na vernietigen gelijk", "[unit][runtime]")
// {
//     Region region("Start");
//     region.createGameObject("A");
//     region.createGameObject("B").destroy();
//     region.createGameObject("C");
//     region.createGameObject("D");
//
//     region.processPendingDestruction();
//
//     CHECK(namesOf(region) == std::vector<std::string>{"A", "C", "D"});
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een id wordt na vernietigen niet opnieuw gebruikt", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& doomed = region.createGameObject("A");
//     const auto oldId = doomed.getId();
//     doomed.destroy();
//     region.processPendingDestruction();
//
//     const auto newId = region.createGameObject("B").getId();
//
//     CHECK(newId != oldId);
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een ouder vernietigen vernietigt ook al zijn nakomelingen", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& parent = region.createGameObject("Ouder");
//     auto& child = region.createGameObject("Kind");
//     auto& grandchild = region.createGameObject("Kleinkind");
//     region.createGameObject("Los");
//     child.setParent(&parent, false);
//     grandchild.setParent(&child, false);
//
//     parent.destroy();
//     region.processPendingDestruction();
//
//     CHECK(namesOf(region) == std::vector<std::string>{"Los"});
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een kind vernietigen laat de ouder en de rest staan", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& parent = region.createGameObject("Ouder");
//     auto& first = region.createGameObject("Kind 1");
//     auto& second = region.createGameObject("Kind 2");
//     first.setParent(&parent, false);
//     second.setParent(&parent, false);
//
//     first.destroy();
//     region.processPendingDestruction();
//
//     CHECK(namesOf(region) == std::vector<std::string>{"Ouder", "Kind 2"});
//     REQUIRE(parent.getChildren().size() == 1);
//     CHECK(parent.getChildren().front() == &second);
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Ouder en kind tegelijk aanvragen geeft geen probleem", "[unit][runtime]")
// {
//     Region region("Start");
//     auto& parent = region.createGameObject("Ouder");
//     auto& child = region.createGameObject("Kind");
//     child.setParent(&parent, false);
//
//     child.destroy();
//     parent.destroy();
//     region.processPendingDestruction();
//
//     CHECK(region.getGameObjects().empty());
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("Een object van een andere Region wordt genegeerd", "[unit][runtime]")
// {
//     Region region("Een");
//     Region other("Twee");
//     auto& foreign = other.createGameObject("Vreemd");
//
//     region.requestDestroy(foreign);
//     region.processPendingDestruction();
//     other.processPendingDestruction();
//
//     CHECK(other.getGameObjects().size() == 1);
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
// TEST_CASE("processPendingDestruction zonder verzoeken doet niets", "[unit][runtime]")
// {
//     Region region("Start");
//     region.createGameObject("A");
//
//     region.processPendingDestruction();
//
//     CHECK(region.getGameObjects().size() == 1);
// }
// TODO: Uncomment de bovenstaande method zodra GameObject beschikbaar is
} // namespace engine
