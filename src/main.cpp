// ============================================================
// main.cpp
// ============================================================
//
// 程序入口。
//
// 当前负责创建核心对象并启动系统。
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
// main 不包含交易逻辑。
// 它只负责程序生命周期管理。
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
    // 所有模块未来通过它通信。
    EventBus eventBus;

    // 创建事件分发器。
    Dispatcher dispatcher(eventBus);

    // 创建交易引擎。
    // 当前使用回测模式。
    Engine engine(
        EngineMode::BACKTEST,
        dispatcher
    );

    engine.start();

    if (engine.running())
    {
        std::cout << "Engine running" << std::endl;
    }

    engine.stop();

    return 0;
}
