#pragma once

#include "TempFile.h"
#include "subsystems/persistence/GameData.h"
#include "subsystems/persistence/ISaveStrategy.h"
#include "platform/IPlatformPaths.h"
#include "runtime/DataNode.h"
#include "subsystems/persistence/GameData.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

namespace engine::subsystems
{
    // Gedeelde testdata voor de unit- en integratietests van persistence.
    // Bouwt een voorbeeld met elk datatype, ook genest.
    inline auto makeSampleData() -> GameData
    {
        using engine::DataNode;

        auto player = DataNode::makeObject();
        player.asObject().emplace("name", DataNode("Hero"));
        player.asObject().emplace("health", DataNode(75));
        player.asObject().emplace("speed", DataNode(3.5));
        player.asObject().emplace("alive", DataNode(true));
        player.asObject().emplace("nickname", DataNode());

        auto positions = DataNode::makeArray();
        positions.asArray().push_back(DataNode(1.5));
        positions.asArray().push_back(DataNode(-2.25));

        auto root = DataNode::makeObject();
        root.asObject().emplace("player", std::move(player));
        root.asObject().emplace("positions", std::move(positions));

        GameData data;
        data.version = 3;
        data.root = std::move(root);
        return data;
    }

    // Paden die niets op schijf aanraken. Voor unit-tests.
    class NoDiskPaths final : public platform::IPlatformPaths
    {
    public:
        [[nodiscard]] std::filesystem::path getAssetRoot() const override { return "assets"; }
        [[nodiscard]] std::filesystem::path getUserDataPath() const override { return "user"; }
    };

    // Een eigen tijdelijke map per test, met een assetmap en een gebruikersmap erin.
    // De map wordt pas aangemaakt als er iets in geschreven wordt, en ruimt zichzelf op.
    class TestPaths final : public platform::IPlatformPaths
    {
    public:
        TestPaths()
            : root(std::filesystem::temp_directory_path() / ("engine_persistence_" + makeSuffix()))
        {
        }
        ~TestPaths() override
        {
            std::error_code ignored;
            std::filesystem::remove_all(root, ignored);
        }
        TestPaths(const TestPaths&) = delete;
        auto operator=(const TestPaths&) -> TestPaths& = delete;
        TestPaths(TestPaths&&) = delete;
        auto operator=(TestPaths&&) -> TestPaths& = delete;

        [[nodiscard]] std::filesystem::path getAssetRoot() const override { return root / "assets"; }
        [[nodiscard]] std::filesystem::path getUserDataPath() const override { return root / "user"; }
        [[nodiscard]] const std::filesystem::path& getRoot() const { return root; }

    private:
        // Uniek per test, ook als tests tegelijk in aparte processen draaien.
        static auto makeSuffix() -> std::string
        {
            std::random_device device;
            const auto high = static_cast<std::uint64_t>(device()) << 32U;
            return std::to_string(high ^ static_cast<std::uint64_t>(device()));
        }

        std::filesystem::path root;
    };

    // Een eigen opslagvorm, om te bewijzen dat de ingestelde strategie gebruikt wordt.
    // Bestandsvorm: "FAKE:<versie>:<tekst>", extensie ".fake".
    class FakeSaveStrategy final : public ISaveStrategy
    {
    public:
        [[nodiscard]] std::string serialize(const GameData& data) const override
        {
            return "FAKE:" + std::to_string(data.version) + ":" + data.root.tryString().value_or("");
        }

        [[nodiscard]] std::optional<GameData> deserialize(const std::string& text) const override
        {
            constexpr std::string_view prefix = "FAKE:";
            if (!text.starts_with(prefix))
                return std::nullopt;
            const auto separator = text.find(':', prefix.size());
            if (separator == std::string::npos)
                return std::nullopt;

            GameData data;
            data.version = static_cast<std::uint32_t>(
                std::stoul(text.substr(prefix.size(), separator - prefix.size())));
            data.root = DataNode(text.substr(separator + 1));
            return data;
        }

        [[nodiscard]] std::string fileExtension() const override { return ".fake"; }
    };

    // Schrijft een bestand en maakt de mappen erboven aan.
    inline void writeFileWithFolders(const std::filesystem::path& file, const std::string& text)
    {
        std::error_code ignored;
        std::filesystem::create_directories(file.parent_path(), ignored);
        writeFile(file, text);
    }
} // namespace engine::subsystems
