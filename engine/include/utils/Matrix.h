#pragma once

#include "utils/Vector2.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <optional>

namespace engine
{
class Matrix
{
public:
    Matrix()
        : elements{1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F}
    {
    }

    explicit Matrix(const std::array<float, 9>& values)
        : elements(values)
    {
    }

    [[nodiscard]] auto at(std::size_t row, std::size_t column) const noexcept -> float
    {
        return elements[row * 3 + column];
    }

    [[nodiscard]] auto at(std::size_t row, std::size_t column) noexcept -> float&
    {
        return elements[row * 3 + column];
    }

    [[nodiscard]] auto transformPoint(const Vector2& point) const noexcept -> Vector2
    {
        return {
            at(0, 0) * point.x + at(0, 1) * point.y + at(0, 2),
            at(1, 0) * point.x + at(1, 1) * point.y + at(1, 2),
        };
    }

    [[nodiscard]] auto operator*(const Matrix& other) const noexcept -> Matrix
    {
        Matrix result{std::array<float, 9>{}};
        for (std::size_t row = 0; row < 3; ++row)
        {
            for (std::size_t column = 0; column < 3; ++column)
            {
                for (std::size_t index = 0; index < 3; ++index)
                {
                    result.at(row, column) += at(row, index) * other.at(index, column);
                }
            }
        }
        return result;
    }

    [[nodiscard]] auto inverse() const noexcept -> std::optional<Matrix>
    {
        const float a = at(0, 0);
        const float b = at(0, 1);
        const float c = at(0, 2);
        const float d = at(1, 0);
        const float e = at(1, 1);
        const float f = at(1, 2);
        const float g = at(2, 0);
        const float h = at(2, 1);
        const float i = at(2, 2);
        const float determinant = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

        if (std::abs(determinant) < 1.0e-6F)
        {
            return std::nullopt;
        }

        const float inverseDeterminant = 1.0F / determinant;
        return Matrix{std::array<float, 9>{
            (e * i - f * h) * inverseDeterminant,
            (c * h - b * i) * inverseDeterminant,
            (b * f - c * e) * inverseDeterminant,
            (f * g - d * i) * inverseDeterminant,
            (a * i - c * g) * inverseDeterminant,
            (c * d - a * f) * inverseDeterminant,
            (d * h - e * g) * inverseDeterminant,
            (b * g - a * h) * inverseDeterminant,
            (a * e - b * d) * inverseDeterminant,
        }};
    }

private:
    std::array<float, 9> elements;
};
} // namespace engine
