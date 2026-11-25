#include "core/EngineImpl.h"

#include "backends/sdl3/SDL3InputSource.h"
#include "facades/InputFacade.h"

#include <iostream>

namespace engine
{
    EngineImpl::EngineImpl(const EngineConfig& config)
    {
        if (config.platformBackend == PlatformBackend::SDL3)
        {
            platform::WindowConfig windowConfig;
            std::unique_ptr<backends::SDL3Window> sdlWindow = std::make_unique<backends::SDL3Window>(windowConfig);
            inputSource_ = std::make_unique<backends::SDL3InputSource>(sdlWindow.get());
            window = std::move(sdlWindow);
            input_ = std::make_unique<subsystems::InputSubsystem>(*inputSource_);
            InputFacade::init(input_.get());
        }
    }

    EngineImpl::~EngineImpl()
    {
        InputFacade::shutdown();
        std::cout << "[Engine] Afsluiten\n";
    }

    void EngineImpl::run()
    {
        // Tijdelijk: er is nog geen input source en geen gameloop, dus run() stopt meteen.
        // Stap 8 vervangt dit door een lus die doorloopt tot er een QuitEvent komt (kruisje).
        std::cout << "[Engine] gestart; nog geen gameloop, run() stopt direct\n";

        while (!input_->isQuitRequested())
        {
            input_->update();
        }
    }
} // namespace engine
