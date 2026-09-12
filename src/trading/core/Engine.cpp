#include "trading/core/Engine.hpp"
#include "trading/core/Dispatcher.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Engine 构造函数
//
// 创建交易系统运行引擎。
//
// Engine 使用外部传入的 Dispatcher。
//
// 原因：
// 不让 Engine 自己创建底层模块。
// 这样：
// - 回测可以注入回放模块
// - 模拟交易可以注入模拟执行模块
// - 实盘可以注入真实连接模块
//
// ============================================================
Engine::Engine(EngineMode mode, Dispatcher& dispatcher)
    : mode_(mode),
      dispatcher_(&dispatcher)
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
