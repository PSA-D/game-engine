#include "../../engine/src/core/GameLoop.h"
#include "core/Engine.h"
#include "core/EngineConfig.h"
#include "facades/InputFacade.h"
#include "runtime/GameObject.h"

#include <cstdlib>
#include <exception>
#include <iostream>

namespace
{
    constexpr engine::Size windowSize{1280, 720};

    class KeyListener : public engine::IKeyListener
    {
    public:
        void onKeyDown(engine::KeyCode key) override
        {
            if (key == engine::KeyCode::Space)
                std::cout << "Jump started" << "\n";
            if (engine::InputFacade::getKey(engine::KeyCode::Left))
                std::cout << "Left" << "\n";
        }
        void onKeyUp(engine::KeyCode key) override
        {
            if (key == engine::KeyCode::Space)
                std::cout << "Jump ended" << "\n";
        }
    };
} // namespace

auto main() -> int
{
    try
    {
        engine::EngineConfig config;
        config.appName = "Dungeon Crawler";
        config.windowSize = windowSize;

        engine::Engine game{config};

        auto listener = std::make_unique<KeyListener>();
        engine::InputFacade::addKeyListener(listener.get());

        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
