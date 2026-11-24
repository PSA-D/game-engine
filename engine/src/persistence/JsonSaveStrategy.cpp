#include "JsonSaveStrategy.h"

#include <cstdint>
#include <limits>
#include <nlohmann/json.hpp>
#include <type_traits>
#include <variant>

namespace engine::subsystem
{
namespace
{
using Json = nlohmann::json;

// Zet een DataNode om naar JSON.
auto toJson(const runtime::DataNode& node) -> Json
{
    return std::visit(
        [](const auto& value) -> Json
        {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return nullptr;
            }
            else if constexpr (std::is_same_v<T, runtime::DataNode::Array>)
            {
                Json array = Json::array();
                for (const auto& element : value)
                    array.push_back(toJson(element));
                return array;
            }
            else if constexpr (std::is_same_v<T, runtime::DataNode::Object>)
            {
                Json object = Json::object();
                for (const auto& [key, element] : value)
                    object[key] = toJson(element);
                return object;
            }
            else
            {
                return value;
                // bool, geheel getal, kommagetal of tekst
            }
        },
        node.getValue());
}

// Zet JSON om naar een DataNode.
auto fromJson(const Json& json) -> runtime::DataNode
{
    using runtime::DataNode;

    switch (json.type())
    {
    case Json::value_t::boolean:
        return DataNode(json.get<bool>());
    case Json::value_t::number_integer:
        return DataNode(json.get<std::int64_t>());
    case Json::value_t::number_unsigned:
    {
        // JSON leest elk positief geheel getal als unsigned. Past het niet in een int64_t,
        // dan bewaren we het als kommagetal (met mogelijk verlies van precisie).
        const auto value = json.get<std::uint64_t>();
        if (value <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()))
            return DataNode(static_cast<std::int64_t>(value));
        return DataNode(static_cast<double>(value));
    }
    case Json::value_t::number_float:
        return DataNode(json.get<double>());
    case Json::value_t::string:
        return DataNode(json.get<std::string>());
    case Json::value_t::array:
    {
        auto node = DataNode::makeArray();
        for (const auto& element : json)
            node.asArray().push_back(fromJson(element));
        return node;
    }
    case Json::value_t::object:
    {
        auto node = DataNode::makeObject();
        for (const auto& [key, element] : json.items())
            node.asObject().emplace(key, fromJson(element));
        return node;
    }
    default: // null, binary en discarded komen niet voor in een geldig bestand
        return {};
    }
}
} // namespace

std::string JsonSaveStrategy::serialize(const GameData& data) const
{
    Json json;
    json["version"] = data.version;
    json["root"] = toJson(data.root);

    // Inspringing van 4 spaties houdt het bestand leesbaar. Met 'replace' gooit ongeldige
    // UTF-8 geen exception.
    return json.dump(4, ' ', false, Json::error_handler_t::replace);
}

std::optional<GameData> JsonSaveStrategy::deserialize(const std::string& text) const
{
    // Het laatste argument (false) zorgt dat een parsefout geen exception gooit.
    const Json json = Json::parse(text, nullptr, false);
    if (json.is_discarded() || !json.is_object())
        return std::nullopt;

    const auto versionIt = json.find("version");
    const auto rootIt = json.find("root");
    if (versionIt == json.end() || rootIt == json.end())
        return std::nullopt;

    if (!versionIt->is_number_unsigned())
        return std::nullopt;

    const auto version = versionIt->get<std::uint64_t>();
    if (version > std::numeric_limits<std::uint32_t>::max())
        return std::nullopt;

    GameData data;
    data.version = static_cast<std::uint32_t>(version);
    data.root = fromJson(*rootIt);
    return data;
}

std::string JsonSaveStrategy::fileExtension() const
{
    return ".json";
}
} // namespace engine::subsystem
