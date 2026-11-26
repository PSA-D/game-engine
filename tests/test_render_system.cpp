#include "Region.h"
#include "backends/null/NullRenderer.h"
#include "backends/sdl3/SDL3Renderer.h"
#include "backends/sdl3/SDL3Window.h"
#include "components/SpriteRenderer.h"
#include "core/GameObject.h"
#include "platform/WindowConfig.h"
#include "systems/RenderSystem.h"
#include "utils/Size.h"

#include <SDL3/SDL.h>
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

        void drawRect(const engine::Rect& rect, const engine::Color& color, bool filled) override
        {
            static_cast<void>(rect);
            static_cast<void>(color);
            static_cast<void>(filled);
            calls.push_back("rect");
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
        renderer.drawRect(engine::Rect{{8.0F, 8.0F}, {24.0F, 24.0F}}, engine::Color{255, 0, 0, 255}, true);
        renderer.drawRect(engine::Rect{{40.0F, 8.0F}, {24.0F, 24.0F}}, engine::Color{0, 255, 0, 255}, false);
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

    try
    {
        engine::platform::WindowConfig config;
        config.title = "SDL3 renderer test";
        config.size = {64, 48};
        config.vsync = false;

        engine::backends::SDL3Window window(config);
        const engine::Size windowPixelSize = window.getPixelSize();
        std::unique_ptr<engine::platform::ITexture> texture;
        {
            engine::backends::SDL3Renderer renderer(window);
            const engine::Vector2 outputSize = renderer.getOutputSize();
            const engine::platform::NativeHandles handles = renderer.getNativeHandles();
            check(outputSize.x == static_cast<float>(windowPixelSize.width) &&
                      outputSize.y == static_cast<float>(windowPixelSize.height),
                  "SDL3Renderer output size matches the window pixel size");
            check(handles.window != nullptr && handles.renderer != nullptr,
                  "SDL3Renderer exposes its native window and renderer");
            int vsync = -1;
            check(SDL_GetRenderVSync(static_cast<SDL_Renderer*>(handles.renderer), &vsync) && vsync == 0,
                  "SDL3Renderer applies the vsync setting from WindowConfig");

            renderer.clear(engine::Color{0, 0, 0, 255});
            renderer.drawRect(engine::Rect{{4.0F, 4.0F}, {16.0F, 16.0F}}, engine::Color{255, 0, 0, 255}, true);
            renderer.drawRect(engine::Rect{{24.0F, 4.0F}, {16.0F, 16.0F}}, engine::Color{0, 255, 0, 255}, false);

            engine::ImageData image;
            image.width = 1;
            image.height = 1;
            image.pixels = {255, 255, 255, 255};
            texture = renderer.createTexture(image);
            check(texture->getWidth() == 1 && texture->getHeight() == 1, "SDL3Texture retains image dimensions");
            renderer.drawTexture(*texture, engine::DrawParams{{4.0F, 24.0F}, {16.0F, 16.0F}, 0.0F});
            renderer.present();
        }
        texture.reset();
    }
    catch (const std::exception& error)
    {
        check(false, error.what());
    }

    if (failures > 0)
    {
        std::cerr << failures << " render test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "All render tests passed\n";
    return EXIT_SUCCESS;
}
