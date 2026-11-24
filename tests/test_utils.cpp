#include "utils/AssetHandle.h"
#include "utils/Color.h"
#include "utils/Interpolator.h"
#include "utils/Matrix.h"
#include "utils/Rect.h"
#include "utils/Vector2.h"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace
{
int failures = 0;

void check(bool condition, const char* message)
{
    if (!condition)
    {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

bool nearlyEqual(float lhs, float rhs)
{
    return std::abs(lhs - rhs) < 1e-5F;
}

template <std::uint32_t Size>
bool nearlyEqual(const engine::Matrix<Size>& lhs, const engine::Matrix<Size>& rhs)
{
    for (std::uint32_t row = 0; row < Size; ++row)
    {
        for (std::uint32_t column = 0; column < Size; ++column)
        {
            if (!nearlyEqual(lhs.at(row, column), rhs.at(row, column)))
            {
                return false;
            }
        }
    }
    return true;
}

void testRect()
{
    const engine::Rect rect{{0.0F, 0.0F}, {10.0F, 10.0F}};

    check(rect.contains(engine::Vector2{5.0F, 5.0F}), "Rect should contain an inner point");
    check(rect.contains(engine::Vector2{0.0F, 0.0F}), "Rect should contain its top-left corner");
    check(!rect.contains(engine::Vector2{10.0F, 5.0F}), "Rect should not contain a point on its right edge");
    check(!rect.contains(engine::Vector2{-1.0F, 5.0F}), "Rect should not contain an outside point");

    check(rect.contains(engine::Rect{{2.0F, 2.0F}, {8.0F, 8.0F}}), "Rect should contain a rect touching its edges");
    check(!rect.contains(engine::Rect{{5.0F, 5.0F}, {10.0F, 10.0F}}), "Rect should not contain a partial overlap");

    check(rect.intersects(engine::Rect{{5.0F, 5.0F}, {10.0F, 10.0F}}), "Overlapping rects should intersect");
    check(!rect.intersects(engine::Rect{{10.0F, 0.0F}, {5.0F, 5.0F}}), "Edge-touching rects should not intersect");
    check(!rect.intersects(engine::Rect{{20.0F, 20.0F}, {5.0F, 5.0F}}), "Separate rects should not intersect");
}

void testColor()
{
    check(engine::Color::WHITE == engine::Color{255, 255, 255, 255}, "WHITE should be opaque white");
    check(engine::Color::BLACK == engine::Color{0, 0, 0, 255}, "BLACK should be opaque black");
    check(engine::Color{}.a == 255, "Default alpha should be opaque");
}

void testAssetHandle()
{
    struct TextureTag;
    using TextureHandle = engine::AssetHandle<TextureTag>;

    const TextureHandle invalid;
    check(!invalid.isValid(), "Default handle should be invalid");
    check(invalid.getIndex() == TextureHandle::INVALID_INDEX, "Default handle should use INVALID_INDEX");

    const TextureHandle handle{3, 7};
    check(handle.isValid(), "Handle with index should be valid");
    check(handle.getIndex() == 3 && handle.getGeneration() == 7, "Handle should store index and generation");
    check(handle != TextureHandle{3, 8}, "Handles with different generations should differ");
}

void testInterpolator()
{
    using engine::Easing;
    using engine::Interpolator;

    check(nearlyEqual(Interpolator::lerp(0.0F, 10.0F, 0.25F), 2.5F), "Float lerp should interpolate");

    const engine::Vector2 mid = Interpolator::lerp(engine::Vector2{0.0F, 0.0F}, engine::Vector2{10.0F, 20.0F}, 0.5F);
    check(nearlyEqual(mid.x, 5.0F) && nearlyEqual(mid.y, 10.0F), "Vector2 lerp should interpolate per axis");

    const engine::Color grey = Interpolator::lerp(engine::Color::BLACK, engine::Color::WHITE, 0.5F);
    check(grey == engine::Color{128, 128, 128, 255}, "Color lerp should round per channel");
    check(Interpolator::lerp(engine::Color::BLACK, engine::Color::WHITE, 2.0F) == engine::Color::WHITE,
          "Color lerp should clamp t");

    for (const Easing easing : {Easing::Linear, Easing::EaseIn, Easing::EaseOut, Easing::EaseInOut})
    {
        check(nearlyEqual(Interpolator::ease(easing, 0.0F), 0.0F), "Easing should start at 0");
        check(nearlyEqual(Interpolator::ease(easing, 1.0F), 1.0F), "Easing should end at 1");
    }
    check(nearlyEqual(Interpolator::ease(Easing::EaseIn, 0.5F), 0.25F), "EaseIn should be quadratic");
    check(nearlyEqual(Interpolator::ease(Easing::EaseOut, 0.5F), 0.75F), "EaseOut should be mirrored quadratic");
    check(nearlyEqual(Interpolator::ease(Easing::EaseInOut, 0.5F), 0.5F), "EaseInOut should be 0.5 halfway");
    check(nearlyEqual(Interpolator::ease(Easing::Linear, 1.5F), 1.0F), "Easing should clamp t");
}

void testMatrix()
{
    const engine::Matrix3 identity;
    const engine::Matrix3 transform{std::array<float, 9>{2.0F, 0.0F, 5.0F, 0.0F, 4.0F, -3.0F, 0.0F, 0.0F, 1.0F}};

    check(identity * transform == transform, "Identity should be neutral for multiplication");

    const auto inverse = transform.inverse();
    check(inverse.has_value(), "Invertible matrix should have an inverse");
    if (inverse.has_value())
    {
        check(nearlyEqual(transform * *inverse, identity), "Matrix times inverse should be identity");
    }

    const engine::Matrix<2> singular{std::array<float, 4>{1.0F, 2.0F, 2.0F, 4.0F}};
    check(!singular.inverse().has_value(), "Singular matrix should have no inverse");

    const engine::Matrix<2> needsPivot{std::array<float, 4>{0.0F, 1.0F, 1.0F, 0.0F}};
    const auto swapped = needsPivot.inverse();
    check(swapped.has_value() && nearlyEqual(*swapped, needsPivot), "Inverse should pivot past a zero diagonal");
}
} // namespace

auto main() -> int
{
    testRect();
    testColor();
    testAssetHandle();
    testInterpolator();
    testMatrix();

    if (failures > 0)
    {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "All utils tests passed\n";
    return EXIT_SUCCESS;
}
