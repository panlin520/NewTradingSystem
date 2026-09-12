// ============================================================
// main.cpp
// ============================================================
//
// 程序入口。
//
// main 只负责创建系统核心组件和启动生命周期。
// 不包含交易逻辑。
//
// 启动流程：
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
//  |
//  v
// Event Loop
//
// 后续扩展：
//
// MarketData
//      |
//      v
// EventBus
//      |
//      v
// Dispatcher
//      |
//      +--> OrderBook
//      +--> Strategy
//      +--> Risk
//      +--> Execution
//
// ============================================================

#include <iostream>

#include "trading/core/Dispatcher.hpp"
#include "trading/core/Engine.hpp"
#include "trading/core/EventBus.hpp"

using namespace CMETradingSystem::Core;


int main()
{
    std::cout << "CMETradingSystem starting..." << std::endl;

    // 创建事件总线。
    // 所有交易模块未来通过 EventBus 解耦通信。
    EventBus eventBus;

    // 创建事件分发器。
    // Dispatcher 负责把事件发送给订阅模块。
    Dispatcher dispatcher(eventBus);

    // 创建交易引擎。
    // 当前运行模式：BACKTEST。
    Engine engine(
        EngineMode::BACKTEST,
        dispatcher
    );

    // 启动 Engine。
    engine.start();

    if (engine.running())
    {
        std::cout << "Engine running" << std::endl;
    }

    // 进入交易系统主循环。
    // 当前 tick() 还没有接入 MarketData。
    // 后续接入 Databento MBO 后，
    // 每一次 tick 会处理一个或多个市场事件。
    engine.run();

    return 0;
}
