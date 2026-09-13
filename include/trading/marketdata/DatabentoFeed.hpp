#pragma once

#include "trading/marketdata/MarketDataFeed.hpp"

#include <string>

namespace CMETradingSystem::MarketData {

// ============================================================
// DatabentoFeed
// ============================================================
//
// CME MDP 3.0 MBO 数据源实现。
//
// 当前阶段：
// - 建立 Databento 专用 Feed 类型
// - 保持与 MarketDataFeed 统一接口
//
// 后续阶段：
// - DBN / DBN.ZST 解码
// - MBO Record 转换为 MarketDataEvent
//
// 不负责：
// - OrderBook 重建
// - Strategy
// - Execution
// ============================================================
class DatabentoFeed final : public MarketDataFeed
{
public:

    explicit DatabentoFeed(std::string file_path);

    ~DatabentoFeed() noexcept override;

    [[nodiscard]] FeedStatus next(MarketDataEvent& event) override;

private:

    std::string file_path_;
};

}
