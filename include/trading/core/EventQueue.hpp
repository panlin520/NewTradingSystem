#pragma once

#include <queue>

#include "trading/core/Event.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// EventQueue
// ============================================================
//
// EventQueue 是交易系统内部的事件等待队列。
//
// 小白理解：
//
// 市场数据不会直接进入 Engine，而是先排队。
//
// 数据流：
//
// MarketData
//      |
//      v
// EventQueue
//      |
//      v
// Engine.tick()
//      |
//      v
// Dispatcher
//      |
//      v
// EventBus
//
// 负责：
// - 保存等待处理的 Event
// - 保证先进先出(FIFO)
// - 提供事件写入和读取接口
//
// 不负责：
// - 行情解析
// - OrderBook重建
// - 策略计算
// - 风控
// ============================================================

class EventQueue
{
public:

    // 添加一个事件到队列尾部。
    void push(const Event& event);

    // 获取并移除队列最前面的事件。
    // 返回：
    // true  = 成功取出事件
    // false = 队列为空
    bool pop(Event& event);

    // 判断当前是否为空。
    bool empty() const;

    // 返回当前事件数量。
    std::size_t size() const;

private:

    // FIFO事件容器。
    std::queue<Event> events_;
};

}
