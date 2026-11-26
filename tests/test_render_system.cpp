#include "Region.h"
#include "backends/null/NullRenderer.h"
#include "components/SpriteRenderer.h"
#include "core/GameObject.h"
#include "systems/RenderSystem.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
    class TestTexture final : public engine::platform::ITexture
    {
    public:
        int getWidth() const noexcept override
        {
            return 16;
        }

        int getHeight() const noexcept override
        {
            return 16;
        }
    };

    class RecordingRenderer final : public engine::platform::IRenderer
    {
    public:
        void clear(const engine::Color& color) override
        {
            static_cast<void>(color);
            calls.push_back("clear");
        }

        void drawTexture(const engine::platform::ITexture& texture, const engine::DrawParams& params) override
        {
            static_cast<void>(texture);
            static_cast<void>(params);
            calls.push_back("draw");
        }

        void present() override
        {
            calls.push_back("present");
        }

        std::unique_ptr<engine::platform::ITexture> createTexture(const engine::ImageData& data) override
        {
            static_cast<void>(data);
            return std::make_unique<TestTexture>();
        }

        [[nodiscard]] engine::Vector2 getOutputSize() const override
        {
            return {800.0F, 600.0F};
        }

        [[nodiscard]] engine::platform::NativeHandles getNativeHandles() const noexcept override
        {
            return {};
        }

        std::vector<std::string> calls;
    };
} // namespace

auto main() -> int
{
    int failures = 0;
    const auto check = [&failures](bool condition, const char* message)
    {
        if (!condition)
        {
            std::cerr << "FAIL: " << message << '\n';
            ++failures;
        }
    };

    {
        RecordingRenderer renderer;
        TestTexture texture;
        engine::Region region;
        engine::GameObject& visibleObject = region.createGameObject("visible");
        visibleObject.addComponent<engine::SpriteRenderer>(texture,
                                                           engine::DrawParams{{32.0F, 48.0F}, {16.0F, 16.0F}, 0.0F});

        engine::GameObject& disabledObject = region.createGameObject("disabled");
        auto& disabledSprite = disabledObject.addComponent<engine::SpriteRenderer>(texture);
        disabledSprite.setEnabled(false);

        engine::RenderSystem renderSystem(renderer);
        renderSystem.render({&visibleObject, nullptr, &disabledObject}, engine::Color{10, 20, 30, 255});
        check(renderer.calls == std::vector<std::string>{"clear", "draw", "present"},
              "RenderSystem clears, draws enabled sprites, then presents");
    }

    {
        engine::backends::NullRenderer renderer;
        const engine::Vector2 outputSize = renderer.getOutputSize();
        const engine::platform::NativeHandles nativeHandles = renderer.getNativeHandles();
        check(outputSize.x == 800.0F && outputSize.y == 600.0F, "NullRenderer reports its fixed output size");
        check(nativeHandles.window == nullptr && nativeHandles.renderer == nullptr,
              "NullRenderer exposes empty native handles");

        engine::ImageData image;
        image.width = 16;
        image.height = 16;
        std::unique_ptr<engine::platform::ITexture> texture = renderer.createTexture(image);
        check(texture->getWidth() == 16 && texture->getHeight() == 16, "NullTexture retains image dimensions");

        renderer.clear(engine::Color{0, 0, 0, 255});
        renderer.drawTexture(*texture, engine::DrawParams{});
        renderer.present();

        const std::unique_ptr<engine::platform::ITexture> emptyTexture = renderer.createTexture(engine::ImageData{});
        check(emptyTexture->getWidth() == 0 && emptyTexture->getHeight() == 0, "NullRenderer accepts empty image data");
        renderer.drawTexture(*emptyTexture, engine::DrawParams{});

        engine::Region region;
        engine::GameObject& object = region.createGameObject("null renderer");
        object.addComponent<engine::SpriteRenderer>(*texture);
        engine::RenderSystem renderSystem(renderer);
        renderSystem.render({&object}, engine::Color{0, 0, 0, 255});
    }

    if (failures > 0)
    {
        std::cerr << failures << " render test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "All render tests passed\n";
    return EXIT_SUCCESS;
}
