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
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

namespace engine
{

    template <std::uint32_t Size>
    class Matrix
    {
        static_assert(Size > 0, "Matrix moet minimaal 1x1 zijn");

    public:
        static constexpr std::uint32_t ARRAY_SIZE = Size * Size;

        constexpr Matrix()
        {
            for (std::uint32_t i = 0; i < Size; ++i)
            {
                at(i, i) = 1.0F;
            }
        }

        explicit constexpr Matrix(const std::array<float, ARRAY_SIZE>& values)
            : elements{values}
        {
        }

        [[nodiscard]] constexpr float at(std::uint32_t row, std::uint32_t column) const
        {
            return elements[toIndex(row, column)];
        }

        [[nodiscard]] constexpr float& at(std::uint32_t row, std::uint32_t column)
        {
            return elements[toIndex(row, column)];
        }

        [[nodiscard]] constexpr const std::array<float, ARRAY_SIZE>& getElements() const
        {
            return elements;
        }

        [[nodiscard]] constexpr Matrix operator*(const Matrix& other) const
        {
            Matrix result{std::array<float, ARRAY_SIZE>{}};
            for (std::uint32_t row = 0; row < Size; ++row)
            {
                for (std::uint32_t column = 0; column < Size; ++column)
                {
                    float sum = 0.0F;
                    for (std::uint32_t k = 0; k < Size; ++k)
                    {
                        sum += at(row, k) * other.at(k, column);
                    }
                    result.at(row, column) = sum;
                }
            }
            return result;
        }

        [[nodiscard]] std::optional<Matrix> inverse() const
        {
            Matrix source = *this;
            Matrix result;

            for (std::uint32_t column = 0; column < Size; ++column)
            {
                std::uint32_t pivot = column;
                for (std::uint32_t row = column + 1; row < Size; ++row)
                {
                    if (std::abs(source.at(row, column)) > std::abs(source.at(pivot, column)))
                    {
                        pivot = row;
                    }
                }

                if (std::abs(source.at(pivot, column)) < SINGULAR_EPSILON)
                {
                    return std::nullopt;
                }

                source.swapRows(pivot, column);
                result.swapRows(pivot, column);

                const float pivotValue = source.at(column, column);
                for (std::uint32_t k = 0; k < Size; ++k)
                {
                    source.at(column, k) /= pivotValue;
                    result.at(column, k) /= pivotValue;
                }

                for (std::uint32_t row = 0; row < Size; ++row)
                {
                    if (row == column)
                    {
                        continue;
                    }

                    const float factor = source.at(row, column);
                    for (std::uint32_t k = 0; k < Size; ++k)
                    {
                        source.at(row, k) -= factor * source.at(column, k);
                        result.at(row, k) -= factor * result.at(column, k);
                    }
                }
            }

            return result;
        }

        friend constexpr bool operator==(const Matrix&, const Matrix&) = default;

    private:
        static constexpr float SINGULAR_EPSILON = 1e-6F;

        static constexpr std::size_t toIndex(std::uint32_t row, std::uint32_t column)
        {
            return (static_cast<std::size_t>(row) * Size) + column;
        }

        constexpr void swapRows(std::uint32_t first, std::uint32_t second)
        {
            if (first == second)
            {
                return;
            }

            for (std::uint32_t k = 0; k < Size; ++k)
            {
                std::swap(at(first, k), at(second, k));
            }
        }

        std::array<float, ARRAY_SIZE> elements{};
    };

    using Matrix3 = Matrix<3>;
} // namespace engine
