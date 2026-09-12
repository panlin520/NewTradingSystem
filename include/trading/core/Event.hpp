#pragma once

#include <cstdint>

namespace CMETradingSystem::Core {

// ============================================================
// EventType（事件类型）
// ============================================================
//
// 用于快速识别事件类别。
//
// 高频交易系统中，事件数量非常大，
// 不使用 dynamic_cast 判断类型，
// 而使用枚举进行快速分发。
//
// 示例：
//
// MarketDataEvent
//        |
//        v
// EventType::MARKET_DATA
//
// ============================================================
enum class EventType
{
    UNKNOWN = 0,

    // CME MDP 3.0 MBO 行情事件。
    MARKET_DATA,

    // 订单相关事件。
    ORDER,

    // 成交相关事件。
    TRADE,

    // 定时器事件。
    TIMER
};


// ============================================================
// Event（事件）基础类型
// ============================================================
//
// 在交易系统中，所有模块之间通过 Event 通信。
//
// 示例：
//
// MarketData
//      |
//      v
// MarketDataEvent
//      |
//      v
// EventBus
//      |
//      +---- OrderBook
//      +---- Strategy
//      +---- Risk
//
// ============================================================
struct Event
{
    // 事件类型。
    //
    // EventBus / Dispatcher 可以根据该字段快速判断事件类别。
    EventType type{EventType::UNKNOWN};


    // 事件发生时间。
    // 高频交易中通常保存纳秒级时间戳。
    uint64_t timestamp{0};


    // 虚析构函数。
    //
    // 支持派生事件：
    //
    // Event
    //  |
    //  +-- MarketDataEvent
    //  +-- OrderEvent
    //  +-- TradeEvent
    //
    virtual ~Event() = default;
};

}
