#include "core/EngineImpl.h"

#include "CompositionRoot.h"
#include "facades/InputFacade.h"

#include <iostream>

namespace engine
{
    EngineImpl::EngineImpl(const EngineConfig& config)
        : factory(core::createPlatformFactory(config.platformBackend))
        , window(factory->createWindow(core::makeWindowConfig(config)))
        , inputSource(factory->createInputSource(*window))
        , input(*inputSource)
        , gameLoop(
              sceneManager, input, physics, animation, audio, render, ai, ui, debug, editor, config.targetFrameRate)
    {
        InputFacade::init(&input);

        const Size size = window->getSize();
        std::cout << "[Engine] Venster aangemaakt: '" << window->getTitle() << "' " << size.width << "x" << size.height
                  << '\n';
    }

    EngineImpl::~EngineImpl()
    {
        InputFacade::shutdown();
        std::cout << "[Engine] Afsluiten\n";
    }

    void EngineImpl::run()
    {
        std::cout << "[Engine] '" << window->getTitle() << "' gestart\n";
        gameLoop.run();
    }
} // namespace engine
