#include "backends/sdl3/SDL3Renderer.h"

#include "backends/sdl3/SDL3Window.h"

#include <SDL3/SDL.h>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace engine::backends
{
    struct SDL3RendererState
    {
        explicit SDL3RendererState(SDL_Renderer* renderer) noexcept
            : renderer(renderer)
        {
        }

        ~SDL3RendererState()
        {
            SDL_DestroyRenderer(renderer);
        }

        SDL_Renderer* renderer;
    };

    namespace
    {
        struct SDLTextureDeleter
        {
            void operator()(SDL_Texture* texture) const noexcept
            {
                SDL_DestroyTexture(texture);
            }
        };

        using SDLTextureHandle = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

        class SDL3Texture final : public platform::ITexture
        {
        public:
            SDL3Texture(std::shared_ptr<SDL3RendererState> rendererState,
                        SDLTextureHandle texture,
                        int textureWidth,
                        int textureHeight) noexcept
                : rendererState(std::move(rendererState))
                , texture(std::move(texture))
                , width(textureWidth)
                , height(textureHeight)
            {
            }

            ~SDL3Texture() override = default;

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
                return texture.get();
            }

        private:
            [[maybe_unused]] std::shared_ptr<SDL3RendererState> rendererState;
            SDLTextureHandle texture;
            const int width;
            const int height;
        };

        auto toSDLRenderer(void* renderer) noexcept -> SDL_Renderer*
        {
            return static_cast<SDL_Renderer*>(renderer);
        }
    } // namespace

    SDL3Renderer::SDL3Renderer(SDL3Window& windowRef)
        : window(windowRef.getNativeHandle())
    {
        const auto destroyRenderer = [](SDL_Renderer* renderer)
        {
            if (renderer != nullptr)
            {
                SDL_DestroyRenderer(renderer);
            }
        };
        std::unique_ptr<SDL_Renderer, decltype(destroyRenderer)> nativeRenderer{
            SDL_CreateRenderer(static_cast<SDL_Window*>(window), nullptr), destroyRenderer};
        if (nativeRenderer == nullptr)
        {
            throw std::runtime_error(SDL_GetError());
        }

        rendererState = std::make_shared<SDL3RendererState>(nativeRenderer.get());
        nativeRenderer.release();
        if (!SDL_SetRenderVSync(rendererState->renderer, windowRef.getConfig().vsync ? 1 : 0))
        {
            throw std::runtime_error(SDL_GetError());
        }
    }

    SDL3Renderer::~SDL3Renderer() = default;

    void SDL3Renderer::clear(const Color& color)
    {
        if (!SDL_SetRenderDrawColor(rendererState->renderer, color.r, color.g, color.b, color.a) ||
            !SDL_RenderClear(rendererState->renderer))
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
        if (!SDL_RenderTextureRotated(rendererState->renderer,
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

    void SDL3Renderer::drawRect(const Rect& rect, const Color& color, bool filled)
    {
        if (!SDL_SetRenderDrawColor(rendererState->renderer, color.r, color.g, color.b, color.a))
        {
            throw std::runtime_error(SDL_GetError());
        }

        const SDL_FRect destination{rect.position.x, rect.position.y, rect.size.x, rect.size.y};
        const bool success = filled ? SDL_RenderFillRect(rendererState->renderer, &destination)
                                    : SDL_RenderRect(rendererState->renderer, &destination);
        if (!success)
        {
            throw std::runtime_error(SDL_GetError());
        }
    }

    void SDL3Renderer::present()
    {
        if (!SDL_RenderPresent(rendererState->renderer))
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

        SDLTextureHandle nativeTexture{SDL_CreateTexture(
            rendererState->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, data.width, data.height)};
        if (nativeTexture == nullptr)
        {
            throw std::runtime_error(SDL_GetError());
        }
        if (!SDL_UpdateTexture(nativeTexture.get(), nullptr, data.pixels.data(), data.width * 4) ||
            !SDL_SetTextureBlendMode(nativeTexture.get(), SDL_BLENDMODE_BLEND))
        {
            throw std::runtime_error(SDL_GetError());
        }
        return std::make_unique<SDL3Texture>(rendererState, std::move(nativeTexture), data.width, data.height);
    }

    Vector2 SDL3Renderer::getOutputSize() const
    {
        int width = 0;
        int height = 0;
        if (!SDL_GetCurrentRenderOutputSize(rendererState->renderer, &width, &height))
        {
            throw std::runtime_error(SDL_GetError());
        }

        return {static_cast<float>(width), static_cast<float>(height)};
    }

    platform::NativeHandles SDL3Renderer::getNativeHandles() const noexcept
    {
        return {window, rendererState->renderer};
    }
} // namespace engine::backends
