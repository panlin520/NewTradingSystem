#include "trading/core/Engine.hpp"
#include "trading/core/Dispatcher.hpp"

namespace CMETradingSystem::Core {

Engine::Engine(
    EngineMode mode,
    Dispatcher& dispatcher,
    EventQueue& event_queue
)
    : mode_(mode),
      dispatcher_(&dispatcher),
      event_queue_(&event_queue)
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


void Engine::run()
{
    while (running())
    {
        tick();
    }
}


// ============================================================
// Engine::tick
//
// 单次系统推进。
//
// 流程：
//
// EventQueue
//      |
//      v
// Engine.tick()
//      |
//      v
// Dispatcher
//      |
//      v
// EventBus
//
// 当前只处理已经进入队列的 Event。
// 不负责产生行情数据。
// ============================================================
void Engine::tick()
{
    if (dispatcher_ == nullptr || event_queue_ == nullptr)
    {
        return;
    }

    Event event;

    if (event_queue_->pop(event))
    {
        dispatcher_->dispatch(event);
    }
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
