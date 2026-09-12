#pragma once

#include "trading/core/EngineMode.hpp"
#include "trading/core/State.hpp"

namespace CMETradingSystem::Core {

class Dispatcher;

class Engine
{
public:
    explicit Engine(EngineMode mode);

    void start();
    void stop();

    bool running() const;
    EngineMode mode() const;

private:
    EngineMode mode_;
    State state_;
    Dispatcher* dispatcher_{nullptr};
};

}
