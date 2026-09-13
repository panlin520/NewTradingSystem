#include "trading/marketdata/MarketDataFeed.hpp"

namespace CMETradingSystem::MarketData {

// ============================================================
// MarketDataFeed destructor
// ============================================================
//
// 将虚析构函数放在 .cpp 中定义，
// 为 MarketDataFeed 提供稳定的类实现入口。
//
// 具体 HistoricalFeed / LiveFeed 都可以通过
// MarketDataFeed 基类安全销毁。
// ============================================================
MarketDataFeed::~MarketDataFeed() noexcept = default;

}
