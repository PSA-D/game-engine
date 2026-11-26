#include "utils/AssetHandle.h"

#include <catch2/catch_test_macros.hpp>

namespace engine
{
    namespace
    {
        struct TextureTag;
        using TextureHandle = AssetHandle<TextureTag>;
    } // namespace

    TEST_CASE("Een nieuwe AssetHandle is ongeldig", "[unit][utils]")
    {
        const TextureHandle handle;

        CHECK_FALSE(handle.isValid());
        CHECK(handle.getIndex() == TextureHandle::INVALID_INDEX);
    }

    TEST_CASE("AssetHandle bewaart index en generatie", "[unit][utils]")
    {
        const TextureHandle handle{3, 7};

        CHECK(handle.isValid());
        CHECK(handle.getIndex() == 3);
        CHECK(handle.getGeneration() == 7);
    }

    TEST_CASE("AssetHandles met een andere generatie zijn verschillend", "[unit][utils]")
    {
        CHECK(TextureHandle{3, 7} == TextureHandle{3, 7});
        CHECK(TextureHandle{3, 7} != TextureHandle{3, 8});
    }
} // namespace engine
