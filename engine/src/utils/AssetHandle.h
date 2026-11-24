#pragma once

#include <cstdint>
#include <limits>

namespace engine
{

template <typename>
class AssetHandle
{
public:
    static constexpr std::uint32_t INVALID_INDEX = std::numeric_limits<std::uint32_t>::max();

    constexpr AssetHandle() = default;

    constexpr AssetHandle(std::uint32_t index, std::uint32_t generation)
        : index_{index}
        , generation_{generation}
    {
    }

    [[nodiscard]] constexpr std::uint32_t getIndex() const
    {
        return index_;
    }

    [[nodiscard]] constexpr std::uint32_t getGeneration() const
    {
        return generation_;
    }

    [[nodiscard]] constexpr bool isValid() const
    {
        return index_ != INVALID_INDEX;
    }

    friend constexpr bool operator==(const AssetHandle&, const AssetHandle&) = default;

private:
    std::uint32_t index_{INVALID_INDEX};
    std::uint32_t generation_{0};
};
} // namespace engine
