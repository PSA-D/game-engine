#include "ComponentRegistry.h"
#include "runtime/DataNode.h"

namespace engine
{
    std::optional<DataNode> ComponentRegistry::toData(
        const Component& component) const
    {
        const auto found = saveEntries.find(typeid(component));

        if (found == saveEntries.end())
        {
            return std::nullopt;
        }

        return found->second.save(component);
    }

    Component* ComponentRegistry::addFromData(
        GameObject& owner,
        const DataNode& data) const
    {
        const auto* typeNode = data.get("type");
        const auto* dataNode = data.get("data");

        if (typeNode == nullptr || dataNode == nullptr)
        {
            return nullptr;
        }

        const auto typeName = typeNode->tryString();

        if (!typeName)
        {
            return nullptr;
        }

        const auto found = loadEntries.find(*typeName);

        if (found == loadEntries.end())
        {
            return nullptr;
        }

        return found->second.load(owner, *dataNode);
    }
} // namespace engine