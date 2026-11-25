#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace engine
{
    // Generieke gegevensboom, vergelijkbaar met JSON, maar zonder afhankelijkheid van een bibliotheek.
    // Een DataNode is leeg (null), een bool, een geheel getal, een kommagetal, een tekst, een lijst of een object met
    // namen.
    class DataNode
    {
    public:
        using Array = std::vector<DataNode>;
        using Object = std::map<std::string, DataNode>;
        using Value = std::variant<std::monostate, bool, std::int64_t, double, std::string, Array, Object>;

        DataNode() = default; // null
        explicit DataNode(bool value)
            : data(value)
        {
        }
        explicit DataNode(int value)
            : data(static_cast<std::int64_t>(value))
        {
        }
        explicit DataNode(std::int64_t value)
            : data(value)
        {
        }
        explicit DataNode(double value)
            : data(value)
        {
        }
        explicit DataNode(std::string value)
            : data(std::move(value))
        {
        }
        // Zonder deze overload kiest de compiler voor een const char* de bool-constructor.
        explicit DataNode(const char* value)
            : data(std::string(value))
        {
        }

        [[nodiscard]] static auto makeArray() -> DataNode
        {
            DataNode node;
            node.data = Array{};
            return node;
        }

        [[nodiscard]] static auto makeObject() -> DataNode
        {
            DataNode node;
            node.data = Object{};
            return node;
        }

        [[nodiscard]] bool isNull() const
        {
            return std::holds_alternative<std::monostate>(data);
        }
        [[nodiscard]] bool isBool() const
        {
            return std::holds_alternative<bool>(data);
        }
        [[nodiscard]] bool isInt() const
        {
            return std::holds_alternative<std::int64_t>(data);
        }
        [[nodiscard]] bool isDouble() const
        {
            return std::holds_alternative<double>(data);
        }
        [[nodiscard]] bool isString() const
        {
            return std::holds_alternative<std::string>(data);
        }
        [[nodiscard]] bool isArray() const
        {
            return std::holds_alternative<Array>(data);
        }
        [[nodiscard]] bool isObject() const
        {
            return std::holds_alternative<Object>(data);
        }

        // Veilig uitlezen: geven std::nullopt (of nullptr) als het type niet klopt, zonder exception.
        // Bedoeld voor het inlezen van bestanden, waar de inhoud niet te vertrouwen is.
        [[nodiscard]] std::optional<bool> tryBool() const
        {
            if (const auto* value = std::get_if<bool>(&data))
                return *value;
            return std::nullopt;
        }
        [[nodiscard]] std::optional<std::int64_t> tryInt() const
        {
            if (const auto* value = std::get_if<std::int64_t>(&data))
                return *value;
            return std::nullopt;
        }
        // Accepteert ook een geheel getal, want in een met de hand bewerkt bestand staat vaak "3".
        [[nodiscard]] std::optional<double> tryDouble() const
        {
            if (const auto* value = std::get_if<double>(&data))
                return *value;
            if (const auto* integer = std::get_if<std::int64_t>(&data))
                return static_cast<double>(*integer);
            return std::nullopt;
        }
        [[nodiscard]] std::optional<std::string> tryString() const
        {
            if (const auto* value = std::get_if<std::string>(&data))
                return *value;
            return std::nullopt;
        }
        [[nodiscard]] const Array* tryArray() const
        {
            return std::get_if<Array>(&data);
        }
        [[nodiscard]] const Object* tryObject() const
        {
            return std::get_if<Object>(&data);
        }

        // Object: een kind opzoeken. Geeft nullptr als dit geen object is of de naam ontbreekt.
        [[nodiscard]] const DataNode* get(const std::string& key) const
        {
            const auto* object = std::get_if<Object>(&data);
            if (object == nullptr)
                return nullptr;
            const auto found = object->find(key);
            return found == object->end() ? nullptr : &found->second;
        }
        [[nodiscard]] DataNode* get(const std::string& key)
        {
            auto* object = std::get_if<Object>(&data);
            if (object == nullptr)
                return nullptr;
            const auto found = object->find(key);
            return found == object->end() ? nullptr : &found->second;
        }

        // Opbouwen. Beide gooien std::bad_variant_access als de node geen object (set) of lijst (pushToArray) is.
        // Begin dus met makeObject() of makeArray().
        void set(std::string key, DataNode child)
        {
            std::get<Object>(data).insert_or_assign(std::move(key), std::move(child));
        }
        void pushToArray(DataNode child)
        {
            std::get<Array>(data).push_back(std::move(child));
        }

        // Voor code die alle typen wil afhandelen, bijvoorbeeld met std::visit.
        [[nodiscard]] const Value& getValue() const
        {
            return data;
        }

        // Deze getters gooien std::bad_variant_access als het type niet klopt.
        // Controleer eerst met isXxx() als je het type niet zeker weet.
        [[nodiscard]] bool asBool() const
        {
            return std::get<bool>(data);
        }
        [[nodiscard]] std::int64_t asInt() const
        {
            return std::get<std::int64_t>(data);
        }
        [[nodiscard]] double asDouble() const
        {
            return std::get<double>(data);
        }
        [[nodiscard]] const std::string& asString() const
        {
            return std::get<std::string>(data);
        }
        [[nodiscard]] const Array& asArray() const
        {
            return std::get<Array>(data);
        }
        [[nodiscard]] Array& asArray()
        {
            return std::get<Array>(data);
        }
        [[nodiscard]] const Object& asObject() const
        {
            return std::get<Object>(data);
        }
        [[nodiscard]] Object& asObject()
        {
            return std::get<Object>(data);
        }

        bool operator==(const DataNode&) const = default;

    private:
        Value data;
    };
} // namespace engine

