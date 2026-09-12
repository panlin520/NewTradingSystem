#include "trading/core/Engine.hpp"
#include "trading/core/Dispatcher.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Engine 构造函数
// ============================================================
//
// 创建交易系统运行引擎。
//
// Engine 使用外部传入的 Dispatcher。
// 不让 Engine 自己创建底层模块。
//
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


// ============================================================
// Engine::run
//
// 交易系统主循环。
//
// 当前版本只建立循环框架。
//
// 后续：
//
// Market Data
//      |
//      v
//    Event
//      |
//      v
// Dispatcher
//      |
//      v
// OrderBook / Strategy / Risk
//
// ============================================================
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
// 当前为空实现。
// 原因：
// 目前还没有接入 MarketData Queue。
//
// 后续负责：
// - 获取行情事件
// - 更新时间
// - 发布 Event
// - 调用 Dispatcher
// ============================================================
void Engine::tick()
{
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
