#include "core/Engine.h"
#include "core/EngineConfig.h"

#include <cstdlib>
#include <exception>
#include <iostream>

namespace
{
    constexpr engine::Size windowSize{1280, 720};
} // namespace

auto main() -> int
{
    try
    {
        engine::EngineConfig config;
        config.appName = "Dungeon Crawler";
        config.windowSize = windowSize;

        engine::Engine game{config};
        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
