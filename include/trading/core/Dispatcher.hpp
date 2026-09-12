#pragma once

#include "trading/core/Event.hpp"
#include "trading/core/EventBus.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Dispatcher
// ============================================================
//
// Dispatcher 是交易系统中的事件分发器。
//
// 小白理解：
//
// 市场中不断产生各种消息：
//
//   行情更新
//   成交事件
//   订单状态变化
//
// 这些消息首先变成 Event，然后通过 Dispatcher 流向对应模块。
//
// 完整流程：
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
// Dispatcher 不负责：
// - 解析行情数据
// - 重建盘口
// - 计算策略信号
// - 执行交易
//
// 它只负责事件流转。
//
// ============================================================

class Dispatcher
{
public:

    // 创建 Dispatcher。
    //
    // 参数：
    //     bus
    //         事件总线。
    //
    // Dispatcher 使用外部传入的 EventBus，
    // 不负责创建和销毁 EventBus。
    explicit Dispatcher(EventBus& bus);


    // 分发一个事件。
    //
    // 流程：
    //
    // Event
    //   |
    //   v
    // Dispatcher
    //   |
    //   v
    // EventBus
    //   |
    //   v
    // 所有订阅模块
    void dispatch(const Event& event);


private:

    // 引用事件总线。
    //
    // 使用引用表示：
    // Dispatcher 不拥有 EventBus 生命周期。
    EventBus& event_bus_;
};

}
