#pragma once

namespace CMETradingSystem::Core {

// ============================================================
// Dispatcher
// ============================================================
//
// Dispatcher 是交易系统中的事件分发器。
//
// 简单理解：
//
//   一个市场事件产生以后，需要找到谁负责处理它。
//   Dispatcher 就负责把事件发送给对应模块。
//
// 未来完整流程：
//
//   Market Data
//        |
//        v
//      Event
//        |
//        v
//    EventBus
//        |
//        v
//   Dispatcher
//        |
//        +---- OrderBook
//        +---- Strategy
//        +---- Risk
//        +---- Execution
//
// 当前版本只是基础接口，后续会接入 EventBus 和事件处理器。
//
// Dispatcher 不负责：
// - 解析行情数据
// - 重建盘口
// - 产生交易信号
// - 执行订单
//
// 它只负责事件流转。
//
// ============================================================

class Dispatcher
{
public:

    // 分发事件。
    //
    // 当前为空实现。
    // 后续会扩展为：
    // Event -> Handler -> Module
    void dispatch();
};

}
