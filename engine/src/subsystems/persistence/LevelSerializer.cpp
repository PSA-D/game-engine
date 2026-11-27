#include "persistence/LevelSerializer.h"

#include "Component.h"
#include "GameObject.h"
#include "Region.h"
#include "Transform.h"
#include "core/ComponentRegistry.h"
#include "runtime/DataNode.h"
#include "runtime/Level.h"
#include "utils/Vector2.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace engine
{
    class Level;
}
namespace engine::subsystems
{
    namespace
    {
        using engine::DataNode;

        // Id in het bestand -> het aangemaakte GameObject, per Region.
        using ObjectsById = std::map<std::uint64_t, GameObject*>;
        // (kind, id van de ouder in het bestand). De ouders worden pas in de tweede ronde gelegd.
        using ParentLinks = std::vector<std::pair<GameObject*, std::uint64_t>>;

        // Een id is een uint64_t, maar DataNode kent alleen int64_t. De cast bewaart alle bits, dus
        // de waarde komt exact terug, ook boven INT64_MAX.
        auto idToNode(std::uint64_t id) -> DataNode
        {
            return DataNode(static_cast<std::int64_t>(id));
        }

        auto nodeToId(const DataNode& node) -> std::optional<std::uint64_t>
        {
            const auto value = node.tryInt();
            if (!value)
                return std::nullopt;
            return static_cast<std::uint64_t>(*value);
        }

        auto vectorToNode(const Vector2& vector) -> DataNode
        {
            auto node = DataNode::makeArray();
            node.pushToArray(DataNode(static_cast<double>(vector.x)));
            node.pushToArray(DataNode(static_cast<double>(vector.y)));
            return node;
        }

        // Verwacht precies twee getallen: [x, y].
        auto nodeToVector(const DataNode& node) -> std::optional<Vector2>
        {
            const auto* list = node.tryArray();
            if (list == nullptr || list->size() != 2)
                return std::nullopt;

            const auto x = (*list)[0].tryDouble();
            const auto y = (*list)[1].tryDouble();
            if (!x || !y)
                return std::nullopt;

            return Vector2{static_cast<float>(*x), static_cast<float>(*y)};
        }

        auto readString(const DataNode& object, const std::string& key) -> std::optional<std::string>
        {
            const auto* child = object.get(key);
            if (child == nullptr)
                return std::nullopt;
            return child->tryString();
        }

        auto readArray(const DataNode& object, const std::string& key) -> const DataNode::Array*
        {
            const auto* child = object.get(key);
            return child == nullptr ? nullptr : child->tryArray();
        }

        // Een veld dat ontbreekt of null is, telt als "niet aanwezig".
        auto readOptional(const DataNode& object, const std::string& key) -> const DataNode*
        {
            const auto* child = object.get(key);
            return (child == nullptr || child->isNull()) ? nullptr : child;
        }

        // ---- Transform (pas deze twee functies aan als de Transform-API anders heet) ----

        auto transformToNode(const Transform& transform) -> DataNode
        {
            auto node = DataNode::makeObject();
            node.set("position", vectorToNode(transform.getLocalPosition()));
            node.set("rotation", DataNode(static_cast<double>(transform.getLocalRotation())));
            node.set("scale", vectorToNode(transform.getLocalScale()));
            return node;
        }

        // Ontbrekende velden houden hun standaardwaarde. Een veld met een verkeerd type is een fout.
        auto applyTransformNode(Transform& transform, const DataNode& node) -> bool
        {
            if (!node.isObject())
                return false;

            if (const auto* position = readOptional(node, "position"))
            {
                const auto value = nodeToVector(*position);
                if (!value)
                    return false;
                transform.setLocalPosition(*value);
            }
            if (const auto* rotation = readOptional(node, "rotation"))
            {
                const auto value = rotation->tryDouble();
                if (!value)
                    return false;
                transform.setLocalRotation(static_cast<float>(*value));
            }
            if (const auto* scale = readOptional(node, "scale"))
            {
                const auto value = nodeToVector(*scale);
                if (!value)
                    return false;
                transform.setLocalScale(*value);
            }
            return true;
        }

        // ---- Opslaan ----

        auto serializeGameObject(const GameObject& gameObject, ComponentRegistry& registry) -> DataNode
        {
            auto node = DataNode::makeObject();
            node.set("id", idToNode(gameObject.getId()));
            node.set("name", DataNode(gameObject.getName()));
            node.set("tag", DataNode(gameObject.getTag()));

            if (const auto* parent = gameObject.getParent())
                node.set("parent", idToNode(parent->getId()));
            else
                node.set("parent", DataNode());

            node.set("transform", transformToNode(gameObject.getTransform()));

            auto components = DataNode::makeArray();
            for (const auto* component : gameObject.getComponents<Component>())
            {
                // Een component dat niet geregistreerd is, kan niet bewaard worden en wordt overgeslagen.
                if (auto data = registry.toData(*component))
                    components.pushToArray(std::move(*data));
            }
            node.set("components", std::move(components));
            return node;
        }

        auto serializeRegion(const Region& region, ComponentRegistry& registry) -> DataNode
        {
            auto gameObjects = DataNode::makeArray();
            for (const auto* gameObject : region.getGameObjects())
                gameObjects.pushToArray(serializeGameObject(*gameObject, registry));

            auto node = DataNode::makeObject();
            node.set("name", DataNode(region.getName()));
            node.set("gameObjects", std::move(gameObjects));
            return node;
        }

        // ---- Inlezen ----

        // Eerste ronde voor één GameObject: aanmaken, tag, transform en componenten.
        // De ouder wordt pas in de tweede ronde gelegd, want die staat misschien verderop in de lijst.
        auto loadGameObject(Region& region,
                            const DataNode& node,
                            ComponentRegistry& registry,
                            ObjectsById& objectsById,
                            ParentLinks& parentLinks) -> bool
        {
            if (!node.isObject())
                return false;

            const auto* idNode = node.get("id");
            const auto id = idNode == nullptr ? std::nullopt : nodeToId(*idNode);
            const auto name = readString(node, "name");
            if (!id || !name || objectsById.contains(*id))
                return false;

            std::string tag;
            if (const auto* tagNode = readOptional(node, "tag"))
            {
                auto value = tagNode->tryString();
                if (!value)
                    return false;
                tag = std::move(*value);
            }

            std::optional<std::uint64_t> parentId;
            if (const auto* parentNode = readOptional(node, "parent"))
            {
                parentId = nodeToId(*parentNode);
                if (!parentId)
                    return false;
            }

            auto& gameObject = region.createGameObject(*name);
            gameObject.setTag(tag);
            objectsById.emplace(*id, &gameObject);
            if (parentId)
                parentLinks.emplace_back(&gameObject, *parentId);

            if (const auto* transformNode = readOptional(node, "transform"))
            {
                if (!applyTransformNode(gameObject.getTransform(), *transformNode))
                    return false;
            }

            if (const auto* componentsNode = readOptional(node, "components"))
            {
                const auto* components = componentsNode->tryArray();
                if (components == nullptr)
                    return false;
                for (const auto& componentNode : *components)
                {
                    // Onbekend type of ongeldige data: het hele level is dan ongeldig.
                    if (registry.addFromData(gameObject, componentNode) == nullptr)
                        return false;
                }
            }
            return true;
        }

        auto loadRegion(Level& level, const DataNode& node, ComponentRegistry& registry) -> Region*
        {
            if (!node.isObject())
                return nullptr;

            const auto name = readString(node, "name");
            const auto* gameObjects = readArray(node, "gameObjects");
            if (!name || gameObjects == nullptr)
                return nullptr;

            auto& region = level.createRegion(*name);
            ObjectsById objectsById;
            ParentLinks parentLinks;

            // Eerste ronde: alle objecten bestaan.
            for (const auto& gameObjectNode : *gameObjects)
            {
                if (!loadGameObject(region, gameObjectNode, registry, objectsById, parentLinks))
                    return nullptr;
            }

            // Tweede ronde: de hiërarchie. De lokale transform blijft zoals bewaard.
            for (const auto& [child, parentId] : parentLinks)
            {
                const auto parent = objectsById.find(parentId);
                if (parent == objectsById.end())
                    return nullptr;
                if (!child->setParent(parent->second, false)) // false: lokale waarden niet aanpassen
                    return nullptr;                           // bijvoorbeeld een cyclus
            }
            return &region;
        }
    } // namespace

    LevelSerializer::LevelSerializer(ComponentRegistry& componentRegistry)
        : registry(componentRegistry)
    {
    }

    GameData LevelSerializer::toGameData(const Level& level) const
    {
        const auto regions = level.getRegions();
        const auto* activeRegion = level.getActiveRegion();

        auto regionNodes = DataNode::makeArray();
        std::optional<std::size_t> activeIndex;
        for (std::size_t index = 0; index < regions.size(); ++index)
        {
            if (regions[index] == activeRegion)
                activeIndex = index;
            regionNodes.pushToArray(serializeRegion(*regions[index], registry));
        }

        auto root = DataNode::makeObject();
        root.set("name", DataNode(level.getName()));
        root.set("regions", std::move(regionNodes));
        if (activeIndex)
            root.set("activeRegion", DataNode(static_cast<std::int64_t>(*activeIndex)));

        GameData data;

        data.version = LevelSerializer::CURRENT_VERSION;
        data.root = std::move(root);
        return data;
    }

    std::unique_ptr<Level> LevelSerializer::fromGameData(const GameData& data) const
    {
        if (data.version == 0 || data.version > CURRENT_VERSION)
            return nullptr; // onbekende of te nieuwe versie

        const auto& root = data.root;
        if (!root.isObject())
            return nullptr;

        const auto name = readString(root, "name");
        const auto* regionNodes = readArray(root, "regions");
        if (!name || regionNodes == nullptr)
            return nullptr;

        // Bij een fout verdwijnt het half gebouwde level vanzelf (unique_ptr).
        auto level = std::make_unique<Level>(*name);
        std::vector<Region*> regions;
        for (const auto& regionNode : *regionNodes)
        {
            auto* region = loadRegion(*level, regionNode, registry);
            if (region == nullptr)
                return nullptr;
            regions.push_back(region);
        }

        if (const auto* activeNode = readOptional(root, "activeRegion"))
        {
            const auto index = activeNode->tryInt();
            if (!index || *index < 0 || static_cast<std::size_t>(*index) >= regions.size())
                return nullptr;
            if (!level->activateRegion(*regions[static_cast<std::size_t>(*index)]))
                return nullptr;
        }
        return level;
    }
} // namespace engine::subsystems
