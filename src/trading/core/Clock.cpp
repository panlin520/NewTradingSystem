#include "trading/core/Clock.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Clock::now
// ============================================================
//
// 当前版本返回固定值 0。
//
// 这是因为目前系统还处于 Core 框架搭建阶段。
//
// 后续实现：
//
// BACKTEST:
//     从历史行情事件读取时间
//
// PAPER/LIVE:
//     从实时行情源读取时间
//
// 统一通过 Clock 提供时间，避免交易模块依赖具体时间来源。

uint64_t Clock::now() const
{
    return 0;
}

}
