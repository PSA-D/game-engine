#include "backends/null/NullRenderer.h"
#include "backends/sdl3/SDL3Renderer.h"
#include "backends/sdl3/SDL3Window.h"
#include "components/SpriteRenderer.h"
#include "core/GameObject.h"
#include "Region.h"
#include "systems/RenderSystem.h"

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>
#include <vector>

namespace
{
auto createTestSprite() -> engine::ImageData
{
    constexpr int spriteSize = 96;
    engine::ImageData image;
    image.width = spriteSize;
    image.height = spriteSize;
    image.pixels.resize(static_cast<std::size_t>(spriteSize * spriteSize * 4));

    for (int y = 0; y < spriteSize; ++y)
    {
        for (int x = 0; x < spriteSize; ++x)
        {
            const bool border = x < 8 || y < 8 || x >= spriteSize - 8 || y >= spriteSize - 8;
            const std::size_t pixel = static_cast<std::size_t>((y * spriteSize + x) * 4);
            image.pixels[pixel] = border ? 255 : 40;
            image.pixels[pixel + 1] = border ? 220 : 130;
            image.pixels[pixel + 2] = border ? 30 : 240;
            image.pixels[pixel + 3] = 255;
        }
    }

    return image;
}

template <typename ShouldContinue>
void runValidation(engine::platform::IRenderer& renderer, ShouldContinue shouldContinue)
{
    std::unique_ptr<engine::platform::ITexture> texture = renderer.createTexture(createTestSprite());
    engine::Region region;
    engine::GameObject& sprite = region.createGameObject("validation sprite");
    sprite.addComponent<engine::SpriteRenderer>(
        *texture, engine::DrawParams{{352.0F, 252.0F}, {96.0F, 96.0F}, 0.0F});
    engine::RenderSystem renderSystem(renderer);
    const std::vector<engine::GameObject*> gameObjects{&sprite};

    while (shouldContinue())
    {
        renderSystem.render(gameObjects, engine::Color{24, 28, 38, 255});
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
} // namespace

auto main(int argc, char* argv[]) -> int
{
    try
    {
        if (argc > 1 && std::string_view(argv[1]) == "--null")
        {
            engine::backends::NullRenderer renderer;
            bool rendered = false;
            runValidation(renderer,
                          [&rendered]
                          {
                              if (rendered)
                              {
                                  return false;
                              }
                              rendered = true;
                              return true;
                          });
            return EXIT_SUCCESS;
        }

        engine::backends::SDL3Window window("Validation Game", 800, 600);
        engine::backends::SDL3Renderer renderer(window);
        runValidation(renderer, [&window] { return window.pollEvents(); });
        return EXIT_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Validation game failed: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
