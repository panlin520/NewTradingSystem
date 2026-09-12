#include "trading/core/EventBus.hpp"

namespace CMETradingSystem::Core {


// ============================================================
// subscribe
// ============================================================
//
// 功能：
// 把一个新的事件处理函数加入 EventBus。
//
// 小白理解：
//
// 就像订阅新闻频道：
//
//   用户 A 订阅行情新闻
//   用户 B 订阅成交新闻
//
// 当新闻发布时，所有订阅者都会收到消息。
//
void EventBus::subscribe(Handler handler)
{
    // 保存这个处理函数。
    // 之后 publish() 发布事件时，会调用它。
    handlers_.push_back(std::move(handler));
}


// ============================================================
// publish
// ============================================================
//
// 功能：
// 发布一个事件，并通知所有订阅者。
//
// 小白理解：
//
// EventBus 不关心事件是谁产生的。
// 它只负责：
//
// 1. 收到事件
// 2. 找到所有监听者
// 3. 转发事件
//
void EventBus::publish(const Event& event) const
{
    // 遍历所有注册的 Handler。
    // 每一个模块都会收到同一个事件。
    for (const auto& handler : handlers_)
    {
        handler(event);
    }
}

}
