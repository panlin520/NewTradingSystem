#pragma once

#include "trading/core/EventQueue.hpp"
#include "trading/marketdata/MarketDataFeed.hpp"

namespace CMETradingSystem::MarketData {

// ============================================================
// FeedAdapter
// ============================================================
//
// 连接 MarketDataFeed 与 Core EventQueue。
//
// 职责：
//
// MarketDataFeed
//        |
//        v
// MarketDataEvent
//        |
//        v
// Core::EventQueue
//
// 不负责：
// - OrderBook
// - Strategy
// - Execution
// - Risk
//
// ============================================================
class FeedAdapter
{
public:

    FeedAdapter(
        MarketDataFeed& feed,
        Core::EventQueue& queue
    );


    // 读取一个行情事件并推入 EventQueue。
    // 返回 true 表示成功产生事件。
    bool poll();

private:

    MarketDataFeed* feed_{nullptr};

    Core::EventQueue* queue_{nullptr};
};

}
