#include "backends/sdl3/SDL3Renderer.h"

#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <cmath>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

namespace engine::backends
{
namespace
{
class SDL3Texture final : public platform::ITexture
{
public:
    SDL3Texture(SDL_Texture* nativeTexture, int textureWidth, int textureHeight) noexcept
        : texture(nativeTexture)
        , width(textureWidth)
        , height(textureHeight)
    {
    }

    ~SDL3Texture() override
    {
        SDL_DestroyTexture(texture);
    }

    [[nodiscard]] int getWidth() const noexcept override
    {
        return width;
    }

    [[nodiscard]] int getHeight() const noexcept override
    {
        return height;
    }

    [[nodiscard]] SDL_Texture* getNativeTexture() const noexcept
    {
        return texture;
    }

private:
    SDL_Texture* texture;
    int width;
    int height;
};

auto toSDLRenderer(void* renderer) noexcept -> SDL_Renderer*
{
    return static_cast<SDL_Renderer*>(renderer);
}
} // namespace

SDL3Renderer::SDL3Renderer(SDL3Window& window)
{
    renderer = SDL_CreateRenderer(static_cast<SDL_Window*>(window.getNativeHandle()), nullptr);
    if (renderer == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

SDL3Renderer::~SDL3Renderer()
{
    SDL_DestroyRenderer(toSDLRenderer(renderer));
}

void SDL3Renderer::clear(const Color& color)
{
    SDL_Renderer* sdlRenderer = toSDLRenderer(renderer);
    if (!SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a) || !SDL_RenderClear(sdlRenderer))
    {
        throw std::runtime_error(SDL_GetError());
    }
}

void SDL3Renderer::drawTexture(const platform::ITexture& texture, const DrawParams& params)
{
    const auto* sdlTexture = dynamic_cast<const SDL3Texture*>(&texture);
    if (sdlTexture == nullptr)
    {
        throw std::invalid_argument("SDL3Renderer requires an SDL3 texture");
    }

    const SDL_FRect destination{params.position.x, params.position.y, params.size.x, params.size.y};
    constexpr double radiansToDegrees = 180.0 / 3.14159265358979323846;
    const double rotation = static_cast<double>(params.rotation) * radiansToDegrees;
    if (!SDL_RenderTextureRotated(toSDLRenderer(renderer),
                                  sdlTexture->getNativeTexture(),
                                  nullptr,
                                  &destination,
                                  rotation,
                                  nullptr,
                                  SDL_FLIP_NONE))
    {
        throw std::runtime_error(SDL_GetError());
    }
}

void SDL3Renderer::present()
{
    if (!SDL_RenderPresent(toSDLRenderer(renderer)))
    {
        throw std::runtime_error(SDL_GetError());
    }
}

std::unique_ptr<platform::ITexture> SDL3Renderer::createTexture(const ImageData& data)
{
    if (data.width <= 0 || data.height <= 0 || data.width > std::numeric_limits<int>::max() / 4)
    {
        throw std::invalid_argument("Texture dimensions must be positive and fit the RGBA pitch");
    }

    const std::size_t width = static_cast<std::size_t>(data.width);
    const std::size_t height = static_cast<std::size_t>(data.height);
    if (width > std::numeric_limits<std::size_t>::max() / height / 4)
    {
        throw std::invalid_argument("Texture dimensions are too large");
    }
    const std::size_t expectedSize = width * height * 4;
    if (data.pixels.size() != expectedSize)
    {
        throw std::invalid_argument("Texture pixel data must contain RGBA pixels for every image pixel");
    }

    SDL_Renderer* sdlRenderer = toSDLRenderer(renderer);
    SDL_Texture* nativeTexture =
        SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, data.width, data.height);
    if (nativeTexture == nullptr)
    {
        throw std::runtime_error(SDL_GetError());
    }
    if (!SDL_UpdateTexture(nativeTexture, nullptr, data.pixels.data(), data.width * 4) ||
        !SDL_SetTextureBlendMode(nativeTexture, SDL_BLENDMODE_BLEND))
    {
        const std::string error = SDL_GetError();
        SDL_DestroyTexture(nativeTexture);
        throw std::runtime_error(error);
    }
    return std::make_unique<SDL3Texture>(nativeTexture, data.width, data.height);
}
} // namespace engine::backends
