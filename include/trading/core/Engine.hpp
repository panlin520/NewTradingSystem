#pragma once

#include "trading/core/EngineMode.hpp"
#include "trading/core/State.hpp"

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
// - 协调核心模块
//
// Engine 不负责：
// - 行情解析
// - OrderBook 重建
// - 策略计算
// - 订单执行
//
// 架构：
//
// main
//  |
//  v
// EventBus
//  |
//  v
// Dispatcher
//  |
//  v
// Engine
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
    //
    // 使用依赖注入，方便未来：
    // BACKTEST / PAPER / LIVE 使用同一套 Engine。
    Engine(EngineMode mode, Dispatcher& dispatcher);


    void start();

    void stop();

    bool running() const;

    EngineMode mode() const;


private:

    EngineMode mode_;

    State state_;

    // 外部传入的事件分发器。
    // Engine 不负责创建它，只负责使用。
    Dispatcher* dispatcher_{nullptr};
};

}
