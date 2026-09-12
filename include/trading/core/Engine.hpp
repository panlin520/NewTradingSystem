#pragma once

#include "trading/core/EngineMode.hpp"
#include "trading/core/State.hpp"
#include "trading/core/EventQueue.hpp"

namespace CMETradingSystem::Core {

class Dispatcher;

// ============================================================
// Engine
// ============================================================
//
// Trading system 的核心运行控制器。
//
// Engine 负责：
// - 管理系统生命周期
// - 保存运行模式(BACKTEST/PAPER/LIVE)
// - 驱动事件循环
// - 从 EventQueue 获取事件并交给 Dispatcher
//
// Engine 不负责：
// - 行情解析
// - OrderBook 重建
// - 策略计算
// - 订单执行
//
// ============================================================

class Engine
{
public:

    // 创建 Engine。
    //
    // 参数：
    // mode       当前运行模式
    // dispatcher 事件分发器
    // event_queue 系统事件队列
    Engine(
        EngineMode mode,
        Dispatcher& dispatcher,
        EventQueue& event_queue
    );

    void start();

    void stop();

    // 运行主事件循环。
    void run();

    // 单次系统推进。
    void tick();

    bool running() const;

    EngineMode mode() const;

private:

    EngineMode mode_;

    State state_;

    // 外部传入的事件分发器。
    Dispatcher* dispatcher_{nullptr};

    // 外部传入的事件队列。
    // Engine 不拥有队列生命周期。
    EventQueue* event_queue_{nullptr};
};

}
