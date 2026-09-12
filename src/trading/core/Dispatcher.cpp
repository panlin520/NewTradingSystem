#include "trading/core/Dispatcher.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Dispatcher::dispatch
// ============================================================
//
// 当前版本：
//
//     空实现
//
// 原因：
// Core 框架目前处于基础搭建阶段。
//
// 后续这里会负责：
//
//     Event
//       |
//       v
//   Dispatcher
//       |
//       v
//   调用对应模块处理函数
//
// 例如：
// - 行情事件 -> OrderBook
// - 成交事件 -> Position/PnL
// - 策略事件 -> Execution
//
// 注意：
// Dispatcher 不应该包含业务逻辑。
// 它只负责消息传递。

void Dispatcher::dispatch()
{
}

}
