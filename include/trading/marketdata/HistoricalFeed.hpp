#pragma once

#include "trading/marketdata/MarketDataFeed.hpp"

#include <cstddef>
#include <string>

namespace CMETradingSystem::MarketData {

// ============================================================
// HistoricalFeed
// ============================================================
//
// 历史行情读取接口。
//
// 当前版本只定义统一接口和生命周期。
// 具体数据解析（例如 Databento DBN MBO）后续实现。
//
// 职责：
// - 读取历史行情数据
// - 按原始顺序输出 MarketDataEvent
//
// 不负责：
// - OrderBook 重建
// - 策略计算
// - 回测逻辑
// - 交易执行
// ============================================================
class HistoricalFeed final : public MarketDataFeed
{
public:

    explicit HistoricalFeed(std::string file_path);

    ~HistoricalFeed() noexcept override;


    // 读取下一条历史行情。
    [[nodiscard]] FeedStatus next(MarketDataEvent& event) override;


private:

    // 数据文件路径。
    std::string file_path_;

    // 当前读取位置。
    std::size_t position_{0};

    // 是否已经到达数据末尾。
    bool end_of_stream_{false};
};

}
