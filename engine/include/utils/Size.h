#pragma once

namespace engine
{
    struct Size
    {
        int width{0};
        int height{0};

        friend constexpr bool operator==(const Size&, const Size&) = default;
    };
} // namespace engine
