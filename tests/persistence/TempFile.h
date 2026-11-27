#pragma once

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <system_error>

namespace engine::subsystems
{
    // Een tijdelijk bestand dat zichzelf opruimt, zodat de tests niets achterlaten.
    class TempFile
    {
    public:
        explicit TempFile(const std::string& name)
            : path(std::filesystem::temp_directory_path() / name)
        {
            std::filesystem::remove(path);
        }
        ~TempFile()
        {
            std::error_code ignored;
            std::filesystem::remove(path, ignored);
        }
        TempFile(const TempFile&) = delete;
        auto operator=(const TempFile&) -> TempFile& = delete;
        TempFile(TempFile&&) = delete;
        auto operator=(TempFile&&) -> TempFile& = delete;

        [[nodiscard]] const std::filesystem::path& getPath() const
        {
            return path;
        }

    private:
        std::filesystem::path path;
    };

    // Binaire modus voorkomt dat Windows regeleindes aanpast.
    inline void writeFile(const std::filesystem::path& path, const std::string& text)
    {
        std::ofstream output(path, std::ios::binary);
        REQUIRE(output.is_open());
        output << text;
    }

    inline auto readFile(const std::filesystem::path& path) -> std::string
    {
        std::ifstream input(path, std::ios::binary);
        REQUIRE(input.is_open());
        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }
} // namespace  engine::subsystems