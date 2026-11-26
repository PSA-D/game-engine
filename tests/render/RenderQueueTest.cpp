#include "systems/render/RenderQueue.h"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <vector>

namespace
{
    class TestTexture final : public engine::platform::ITexture
    {
    public:
        int getWidth() const noexcept override
        {
            return 1;
        }

        int getHeight() const noexcept override
        {
            return 1;
        }
    };

    auto makeItem(const engine::platform::ITexture& texture, int layer, int order, float marker) -> engine::RenderItem
    {
        engine::RenderItem item;
        item.texture = &texture;
        item.sortingLayer = layer;
        item.orderInLayer = order;
        item.position.x = marker;
        return item;
    }
} // namespace

TEST_CASE("RenderQueue sorts by sorting layer, order and texture", "[render][queue]")
{
    TestTexture firstTexture;
    TestTexture secondTexture;
    const std::less<const engine::platform::ITexture*> textureLess;
    const auto* lowerTexture = textureLess(&firstTexture, &secondTexture) ? &firstTexture : &secondTexture;
    const auto* higherTexture = textureLess(&firstTexture, &secondTexture) ? &secondTexture : &firstTexture;

    engine::RenderQueue queue;
    queue.add(makeItem(*higherTexture, 1, 0, 0.0F));
    queue.add(makeItem(*lowerTexture, 2, 0, 1.0F));
    queue.add(makeItem(*lowerTexture, 1, 2, 2.0F));
    queue.add(makeItem(*lowerTexture, 1, 1, 3.0F));
    queue.add(makeItem(*lowerTexture, 1, 0, 4.0F));
    queue.sort();

    const auto& items = queue.getItems();
    REQUIRE(items.size() == 5);
    CHECK(items[0].position.x == 4.0F);
    CHECK(items[1].position.x == 0.0F);
    CHECK(items[2].position.x == 3.0F);
    CHECK(items[3].position.x == 2.0F);
    CHECK(items[4].position.x == 1.0F);
}

TEST_CASE("RenderQueue keeps submission order for equal sort keys", "[render][queue]")
{
    TestTexture texture;
    engine::RenderQueue queue;
    queue.add(makeItem(texture, 1, 2, 10.0F));
    queue.add(makeItem(texture, 1, 2, 20.0F));
    queue.add(makeItem(texture, 1, 2, 30.0F));

    queue.sort();

    const auto& items = queue.getItems();
    REQUIRE(items.size() == 3);
    CHECK(items[0].position.x == 10.0F);
    CHECK(items[1].position.x == 20.0F);
    CHECK(items[2].position.x == 30.0F);
}

TEST_CASE("RenderQueue supports empty queues and clear", "[render][queue]")
{
    engine::RenderQueue queue;
    CHECK(queue.getItems().empty());
    queue.sort();
    CHECK(queue.getItems().empty());

    TestTexture texture;
    queue.add(makeItem(texture, 0, 0, 0.0F));
    queue.clear();
    CHECK(queue.getItems().empty());
}
