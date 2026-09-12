#pragma once

#include "trading/core/EngineMode.hpp"
#include "trading/core/State.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Engine
// ============================================================
//
// Trading system 的核心运行控制器。
//
// 可以把 Engine 理解为整个交易程序的总开关：
//
//     main.cpp
//          |
//          v
//        Engine
//          |
//          +---- State (保存当前运行状态)
//          |
//          +---- Dispatcher (未来负责事件分发)
//
// 在完整交易系统中，Engine 不负责：
//
// - 行情解析
// - 订单簿重建
// - 策略计算
// - 下单执行
//
// 它只负责协调系统生命周期。
//
// 例如：
//
// BACKTEST 模式：
//     启动历史数据回放
//
// PAPER 模式：
//     启动模拟交易
//
// LIVE 模式：
//     启动真实交易连接
//
// ============================================================

class Dispatcher;

class Engine
{
public:

    // 创建交易系统 Engine。
    //
    // 参数：
    //     mode
    //         当前运行模式：
    //         BACKTEST / PAPER / LIVE
    //
    explicit Engine(EngineMode mode);


    // 启动交易系统。
    //
    // 当前阶段：
    //     修改 State.running 状态。
    //
    // 后续会扩展：
    //     启动数据流
    //     启动事件循环
    //     启动策略模块
    void start();


    // 停止交易系统。
    //
    // 后续会负责：
    //     停止行情接收
    //     保存状态
    //     安全关闭模块
    void stop();


    // 查询 Engine 是否正在运行。
    bool running() const;


    // 获取当前运行模式。
    EngineMode mode() const;


private:

    // 当前运行模式。
    //
    // 决定系统运行环境：
    // BACKTEST / PAPER / LIVE
    EngineMode mode_;


    // 系统状态。
    //
    // 当前保存：
    //     running 状态
    //
    // 后续可能扩展：
    //     连接状态
    //     风控状态
    //     系统健康状态
    State state_;


    // 事件分发器。
    //
    // 当前只是预留接口。
    //
    // 后续负责连接：
    //
    // MarketData Event
    //        |
    //        v
    // Dispatcher
    //        |
    //        v
    // OrderBook / Strategy / Risk
    //
    Dispatcher* dispatcher_{nullptr};
};

}
