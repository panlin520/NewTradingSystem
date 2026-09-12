#pragma once

#include <cstdint>

namespace CMETradingSystem::Core {

// ============================================================
// Event（事件）基础类型
// ============================================================
//
// 在交易系统中，很多事情都会发生：
//
// 例如：
// - 收到行情数据
// - 订单成交
// - 定时器触发
// - 策略产生交易信号
//
// 这些发生的事情，都可以抽象成一个 Event。
//
// 后续系统中的不同模块，不直接互相调用，
// 而是通过 Event 进行通信。
//
// 示例流程：
//
// MarketData
//      |
//      v
//  MarketEvent
//      |
//      v
//  EventBus
//      |
//      +----> OrderBook
//      +----> Strategy
//      +----> Risk
//
// 这样可以降低模块之间的耦合。
// ============================================================
struct Event
{
    // 事件发生的时间。
    //
    // 当前使用 uint64_t 保存时间戳。
    // 在高频交易系统中通常使用纳秒级时间。
    uint64_t timestamp{0};

    // 虚析构函数。
    //
    // 作用：
    // 允许未来创建不同类型的事件继承 Event。
    //
    // 例如：
    // class MarketDataEvent : public Event
    // class OrderEvent : public Event
    //
    // 删除基类指针对象时，可以正确调用子类析构函数。
    virtual ~Event() = default;
};

}
