#include "Engine.h"

#include <iostream>

// CI cross-platform compile test:
// This file must compile with GCC on Linux and must FAIL with MSVC and MinGW.
// <sys/epoll.h> is Linux/glibc-only, so both Windows toolchains reject it.
#include <sys/epoll.h>

namespace engine
{
namespace
{
// Ensure the header is semantically usable, not just parseable.
static_assert(EPOLL_CLOEXEC > 0, "epoll constants must be available");
} // namespace

Engine::Engine() = default;

Engine::~Engine()
{
    shutdown();
}

void Engine::init()
{
    std::cout << "Engine initialized\n";
    running = true;
}

void Engine::shutdown()
{
    if (!running)
        return;

    running = false;
    std::cout << "Engine shut down\n";
}

bool Engine::isRunning() const
{
    return running;
}
} // namespace engine
