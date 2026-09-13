#pragma once

#include "trading/marketdata/MarketDataEvent.hpp"

namespace CMETradingSystem::MarketData {

// ============================================================
// FeedStatus
// ============================================================
//
// MarketDataFeed 每次尝试读取行情后的统一结果。
//
// 这样 HistoricalFeed 和 LiveFeed 可以共用同一套接口：
//
// HistoricalFeed:
//     EVENT         -> 成功读取一个历史事件
//     END_OF_STREAM -> 历史数据已经读取完成
//     ERROR         -> 数据读取或解析失败
//
// LiveFeed:
//     EVENT         -> 成功收到一个实时事件
//     IDLE          -> 当前暂时没有新事件
//     ERROR         -> 实时数据源发生错误
//
// ============================================================
enum class FeedStatus
{
    EVENT,
    IDLE,
    END_OF_STREAM,
    ERROR
};


// ============================================================
// MarketDataFeed
// ============================================================
//
// 统一行情源接口。
//
// BACKTEST / PAPER / LIVE 不允许使用不同的 MarketDataEvent。
// 不同模式只允许替换具体 Feed 实现。
//
// 典型结构：
//
// HistoricalFeed ----+
//                    |
// LiveFeed ----------+----> MarketDataFeed
//                              |
//                              v
//                       MarketDataEvent
//                              |
//                              v
//                        Replay / Engine
//
// MarketDataFeed 只负责：
// - 从具体数据源读取行情
// - 输出统一 MarketDataEvent
// - 返回明确的读取状态
//
// MarketDataFeed 不负责：
// - OrderBook 重建
// - Feature 计算
// - Strategy
// - Risk
// - Execution
// - Portfolio / PnL
//
// ============================================================
class MarketDataFeed
{
public:

    // 使用虚析构函数，确保通过基类指针销毁具体 Feed 时安全。
    virtual ~MarketDataFeed() noexcept;


    // --------------------------------------------------------
    // next
    // --------------------------------------------------------
    //
    // 尝试读取下一个统一行情事件。
    //
    // 参数：
    // event
    //     调用成功并返回 FeedStatus::EVENT 时，
    //     由具体 Feed 填充完整 MarketDataEvent。
    //
    // 返回值：
    // FeedStatus::EVENT
    //     已产生一个有效事件。
    //
    // FeedStatus::IDLE
    //     当前暂时没有事件，主要用于实时 Feed。
    //
    // FeedStatus::END_OF_STREAM
    //     历史数据已经读取完成。
    //
    // FeedStatus::ERROR
    //     当前 Feed 发生错误。
    //
    // 这里使用调用方提供的 MarketDataEvent，避免接口层为每个
    // 行情事件强制进行一次堆内存分配。
    // --------------------------------------------------------
    [[nodiscard]] virtual FeedStatus next(MarketDataEvent& event) = 0;
};

}
