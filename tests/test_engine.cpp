#include "Engine.h"

#include <cstdlib>
#include <iostream>

auto main() -> int
{
    int failures = 0;

    {
        engine::Engine engine;
        engine.init();
        if (!engine.isRunning())
        {
            std::cerr << "FAIL: Engine should be running after init\n";
            failures++;
        }
    }

    {
        engine::Engine engine;
        if (engine.isRunning())
        {
            std::cerr << "FAIL: Engine should not be running before init\n";
            failures++;
        }
    }

    {
        engine::Engine engine;
        engine.init();
        engine.shutdown();
        if (engine.isRunning())
        {
            std::cerr << "FAIL: Engine should not be running after shutdown\n";
            failures++;
        }
    }

    if (failures > 0)
    {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "All tests passed\n";
    return EXIT_SUCCESS;
}
