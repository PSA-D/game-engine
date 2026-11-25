#pragma once

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
