#include "utils/Matrix.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstdint>

using Catch::Matchers::WithinAbs;

namespace engine
{
    namespace
    {
        constexpr double TOLERANCE = 1e-5;

        template <std::uint32_t Size>
        void checkNearlyEqual(const Matrix<Size>& actual, const Matrix<Size>& expected)
        {
            for (std::uint32_t row = 0; row < Size; ++row)
            {
                for (std::uint32_t column = 0; column < Size; ++column)
                {
                    CHECK_THAT(actual.at(row, column), WithinAbs(expected.at(row, column), TOLERANCE));
                }
            }
        }
    } // namespace

    TEST_CASE("Een nieuwe Matrix is de identiteitsmatrix", "[unit][utils]")
    {
        const Matrix3 matrix;

        CHECK(matrix == Matrix3{std::array<float, 9>{1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F}});
    }

    TEST_CASE("Vermenigvuldigen met de identiteitsmatrix verandert niets", "[unit][utils]")
    {
        const Matrix3 transform{std::array<float, 9>{2.0F, 0.0F, 5.0F, 0.0F, 4.0F, -3.0F, 0.0F, 0.0F, 1.0F}};

        CHECK(Matrix3{} * transform == transform);
        CHECK(transform * Matrix3{} == transform);
    }

    TEST_CASE("Een matrix keer zijn inverse is de identiteitsmatrix", "[unit][utils]")
    {
        const Matrix3 transform{std::array<float, 9>{2.0F, 0.0F, 5.0F, 0.0F, 4.0F, -3.0F, 0.0F, 0.0F, 1.0F}};

        const auto inverse = transform.inverse();

        REQUIRE(inverse.has_value());
        checkNearlyEqual(transform * *inverse, Matrix3{});
    }

    TEST_CASE("Een singuliere matrix heeft geen inverse", "[unit][utils]")
    {
        const Matrix<2> singular{std::array<float, 4>{1.0F, 2.0F, 2.0F, 4.0F}};

        CHECK_FALSE(singular.inverse().has_value());
    }

    TEST_CASE("inverse werkt ook met een nul op de diagonaal", "[unit][utils]")
    {
        const Matrix<2> swap{std::array<float, 4>{0.0F, 1.0F, 1.0F, 0.0F}};

        const auto inverse = swap.inverse();

        REQUIRE(inverse.has_value());
        checkNearlyEqual(*inverse, swap);
    }
} // namespace engine
