#include "trading/core/Dispatcher.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// Dispatcher 构造函数
// ============================================================
//
// 作用：
// 保存外部传入的 EventBus。
//
// 设计原因：
//
// Dispatcher 不应该自己创建 EventBus。
//
// 因为未来：
//
// BACKTEST:
//     ReplayData -> EventBus
//
// PAPER:
//     Simulator -> EventBus
//
// LIVE:
//     CME Market Data -> EventBus
//
// 都应该使用同一套事件分发流程。
// ============================================================
Dispatcher::Dispatcher(EventBus& bus)
    : event_bus_(bus)
{
}


// ============================================================
// Dispatcher::dispatch
// ============================================================
//
// 功能：
// 接收事件，然后发送给 EventBus。
//
// 流程：
//
//     Event
//       |
//       v
//   Dispatcher
//       |
//       v
//   EventBus
//       |
//       +---- OrderBook
//       +---- Strategy
//       +---- Risk
//
// 注意：
// Dispatcher 不处理业务逻辑。
// 它只是消息转发层。
// ============================================================
void Dispatcher::dispatch(const Event& event)
{
    event_bus_.publish(event);
}

}
