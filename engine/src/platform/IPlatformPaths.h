#pragma once

#include <filesystem>

namespace engine::platform
{
    // Vaste mappen van het platform: waar de spelbestanden staan en waar de speler zijn savegames bewaart.
    class IPlatformPaths
    {
    public:
        IPlatformPaths() = default;
        virtual ~IPlatformPaths() = default;
        IPlatformPaths(const IPlatformPaths&) = delete;
        auto operator=(const IPlatformPaths&) -> IPlatformPaths& = delete;
        IPlatformPaths(IPlatformPaths&&) = delete;
        auto operator=(IPlatformPaths&&) -> IPlatformPaths& = delete;

        // Map met de meegeleverde bestanden (levels, afbeeldingen, geluid). Alleen om te lezen.
        [[nodiscard]] virtual std::filesystem::path getAssetRoot() const = 0;

        // Map waar de speler zijn eigen bestanden bewaart (savegames, instellingen).
        [[nodiscard]] virtual std::filesystem::path getUserDataPath() const = 0;
    };
} // namespace engine::platform