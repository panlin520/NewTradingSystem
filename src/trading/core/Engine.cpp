#include "trading/core/Engine.hpp"

namespace CMETradingSystem::Core {

Engine::Engine(EngineMode mode)
    : mode_(mode)
{
}

void Engine::start()
{
    state_.running = true;
}

void Engine::stop()
{
    state_.running = false;
}

bool Engine::running() const
{
    return state_.running;
}

EngineMode Engine::mode() const
{
    return mode_;
}

}
