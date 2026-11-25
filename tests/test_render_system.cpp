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
        engine::GameObject visibleObject;
        visibleObject.addComponent<engine::SpriteRenderer>(texture,
                                                           engine::DrawParams{{32.0F, 48.0F}, {16.0F, 16.0F}, 0.0F});

        engine::GameObject disabledObject;
        auto& disabledSprite = disabledObject.addComponent<engine::SpriteRenderer>(texture);
        disabledSprite.setEnabled(false);

        engine::RenderSystem renderSystem(renderer);
        renderSystem.render({&visibleObject, nullptr, &disabledObject}, engine::Color{10, 20, 30, 255});
        check(renderer.calls == std::vector<std::string>{"clear", "draw", "present"},
              "RenderSystem clears, draws enabled sprites, then presents");
    }

    {
        engine::backends::NullRenderer renderer;
        engine::ImageData image;
        image.width = 16;
        image.height = 16;
        std::unique_ptr<engine::platform::ITexture> texture = renderer.createTexture(image);
        engine::GameObject object;
        object.addComponent<engine::SpriteRenderer>(*texture);
        engine::RenderSystem renderSystem(renderer);
        renderSystem.render({&object}, engine::Color{0, 0, 0, 255});
        check(texture->getWidth() == 16 && texture->getHeight() == 16,
              "the same render system works with the Null renderer");
    }

    if (failures > 0)
    {
        std::cerr << failures << " render test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "All render tests passed\n";
    return EXIT_SUCCESS;
}
