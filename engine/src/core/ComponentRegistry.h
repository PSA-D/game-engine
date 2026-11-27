#pragma once

#include "runtime/Component.h"
#include "runtime/DataNode.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace engine
{
    class GameObject;

    class ComponentRegistry
    {
    public:
        using SaveFunction = std::function<std::optional<DataNode>(const Component&)>;

        using LoadFunction = std::function<Component*(GameObject&, const DataNode&)>;

        template <typename T>
        using SaveComponentFunction = std::function<std::optional<DataNode>(const T&)>;

        template <typename T>
        using LoadComponentFunction = std::function<T*(GameObject&, const DataNode&)>;

        template <typename T>
        void registerType(const std::string& typeName, SaveComponentFunction<T> save, LoadComponentFunction<T> load)
        {
            saveEntries[typeid(T)] = {typeName,
                                      [typeName, save](const Component& component) -> std::optional<DataNode>
                                      {
                                          const auto* typed = dynamic_cast<const T*>(&component);

                                          if (typed == nullptr)
                                          {
                                              return std::nullopt;
                                          }

                                          auto componentData = save(*typed);

                                          if (!componentData)
                                          {
                                              return std::nullopt;
                                          }

                                          auto node = DataNode::makeObject();
                                          node.set("type", DataNode(typeName));
                                          node.set("data", std::move(*componentData));
                                          node.set("enabled", DataNode(component.isEnabled()));

                                          return node;
                                      }};

            loadEntries[typeName] = {[load](GameObject& owner, const DataNode& data) -> Component*
                                     { return load(owner, data); }};
        }

        [[nodiscard]]
        std::optional<DataNode> toData(const Component& component) const;

        [[nodiscard]]
        Component* addFromData(GameObject& owner, const DataNode& data) const;

    private:
        struct SaveEntry
        {
            std::string typeName;
            SaveFunction save;
        };

        struct LoadEntry
        {
            LoadFunction load;
        };

        std::unordered_map<std::type_index, SaveEntry> saveEntries;
        std::unordered_map<std::string, LoadEntry> loadEntries;
    };

} // namespace engine
