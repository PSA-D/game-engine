#include "core/EngineImpl.h"

#include "core/CompositionRoot.h"

#include <iostream>

namespace engine
{
    EngineImpl::EngineImpl(const EngineConfig& config)
        : factory(core::createPlatformFactory(config.platformBackend))
        , window(factory->createWindow(core::makeWindowConfig(config)))
    {
        const Size size = window->getSize();
        std::cout << "[Engine] Venster aangemaakt: '" << window->getTitle() << "' " << size.width << "x" << size.height
                  << '\n';
    }

    EngineImpl::~EngineImpl()
    {
        // De members worden hierna in omgekeerde volgorde opgeruimd: window, daarna factory.
        std::cout << "[Engine] Afsluiten\n";
    }

    void EngineImpl::run()
    {
        // Tijdelijk: er is nog geen input source en geen gameloop, dus run() stopt meteen.
        // Stap 8 vervangt dit door een lus die doorloopt tot er een QuitEvent komt (kruisje).
        std::cout << "[Engine] '" << window->getTitle() << "' gestart; nog geen gameloop, run() stopt direct\n";
    }
} // namespace engine
