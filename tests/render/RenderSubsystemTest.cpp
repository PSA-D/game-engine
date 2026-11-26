#include "subsystems/render/RenderSubsystem.h"

#include "platform/ImageData.h"

#include <catch2/catch_test_macros.hpp>
#include <memory>
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

    class FakeRenderer final : public engine::platform::IRenderer
    {
    public:
        struct DrawCall
        {
            const engine::platform::ITexture* texture;
            engine::DrawParams params;
        };

        void clear(const engine::Color& color) override
        {
            clearColors.push_back(color);
        }

        void drawTexture(const engine::platform::ITexture& texture, const engine::DrawParams& params) override
        {
            drawCalls.push_back({&texture, params});
        }

        void drawRect(const engine::Rect& rect, const engine::Color& color, bool filled) override
        {
            static_cast<void>(rect);
            static_cast<void>(color);
            static_cast<void>(filled);
        }

        void present() override
        {
            ++presentCount;
        }

        std::unique_ptr<engine::platform::ITexture> createTexture(const engine::ImageData& data) override
        {
            static_cast<void>(data);
            return std::make_unique<TestTexture>();
        }

        [[nodiscard]] engine::Vector2 getOutputSize() const override
        {
            return outputSize;
        }

        [[nodiscard]] engine::platform::NativeHandles getNativeHandles() const noexcept override
        {
            return {};
        }

        engine::Vector2 outputSize{200.0F, 100.0F};
        std::vector<engine::Color> clearColors;
        std::vector<DrawCall> drawCalls;
        int presentCount{0};
    };

    auto makeItem(const engine::platform::ITexture& texture, const engine::Vector2& position) -> engine::RenderItem
    {
        engine::RenderItem item;
        item.texture = &texture;
        item.position = position;
        item.size = {10.0F, 10.0F};
        item.pivot = {0.0F, 0.0F};
        return item;
    }
} // namespace

TEST_CASE("RenderSubsystem transforms world coordinates through viewport and zoom", "[render][subsystem]")
{
    FakeRenderer renderer;
    engine::subsystems::RenderSubsystem subsystem(renderer);
    TestTexture texture;

    const engine::Color clearColor{12, 34, 56, 255};
    subsystem.setClearColor(clearColor);
    subsystem.beginFrame();

    engine::CameraView camera;
    camera.position = {10.0F, 20.0F};
    camera.zoom = 2.0F;
    camera.viewport = {{0.25F, 0.2F}, {0.5F, 0.5F}};
    subsystem.submitCamera(camera);
    subsystem.submit(makeItem(texture, {12.0F, 20.0F}));
    subsystem.submit(makeItem(texture, {1000.0F, 20.0F}));

    subsystem.render();

    REQUIRE(renderer.clearColors.size() == 1);
    CHECK(renderer.clearColors[0].r == clearColor.r);
    CHECK(renderer.clearColors[0].g == clearColor.g);
    CHECK(renderer.clearColors[0].b == clearColor.b);
    CHECK(renderer.clearColors[0].a == clearColor.a);
    REQUIRE(renderer.drawCalls.size() == 1);
    CHECK((renderer.drawCalls[0].params.position == engine::Vector2{104.0F, 45.0F}));
    CHECK((renderer.drawCalls[0].params.size == engine::Vector2{20.0F, 20.0F}));

    subsystem.endFrame();
    CHECK(renderer.presentCount == 1);
}

TEST_CASE("RenderSubsystem renders cameras in priority order", "[render][subsystem]")
{
    FakeRenderer renderer;
    renderer.outputSize = {100.0F, 100.0F};
    engine::subsystems::RenderSubsystem subsystem(renderer);
    TestTexture texture;

    subsystem.beginFrame();
    subsystem.submit(makeItem(texture, {0.0F, 0.0F}));

    engine::CameraView laterCamera;
    laterCamera.position = {20.0F, 0.0F};
    laterCamera.priority = 10;
    subsystem.submitCamera(laterCamera);

    engine::CameraView earlierCamera;
    earlierCamera.priority = 0;
    subsystem.submitCamera(earlierCamera);

    subsystem.render();

    REQUIRE(renderer.drawCalls.size() == 2);
    CHECK(renderer.drawCalls[0].params.position.x == 50.0F);
    CHECK(renderer.drawCalls[1].params.position.x == 30.0F);
    subsystem.endFrame();
}

TEST_CASE("RenderSubsystem clears submitted items and cameras at endFrame", "[render][subsystem]")
{
    FakeRenderer renderer;
    renderer.outputSize = {100.0F, 100.0F};
    engine::subsystems::RenderSubsystem subsystem(renderer);
    TestTexture texture;
    engine::CameraView camera;

    subsystem.beginFrame();
    subsystem.submit(makeItem(texture, {0.0F, 0.0F}));
    subsystem.submitCamera(camera);
    subsystem.render();
    subsystem.endFrame();

    subsystem.render();
    CHECK(renderer.drawCalls.size() == 1);

    subsystem.beginFrame();
    subsystem.render();
    subsystem.endFrame();

    CHECK(renderer.drawCalls.size() == 1);
    CHECK(renderer.presentCount == 2);
}
