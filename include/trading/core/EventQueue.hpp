#pragma once

#include <memory>
#include <queue>

#include "trading/core/Event.hpp"

namespace CMETradingSystem::Core {

// ============================================================
// EventQueue
// ============================================================
//
// 多态事件队列。
//
// 为什么使用 unique_ptr<Event>：
//
// Event 是所有事件的基类。
// 例如：
//
// Event
//   |
//   +-- MarketDataEvent
//   +-- OrderEvent
//   +-- TimerEvent
//
// 如果直接保存 Event，会发生对象切片，
// 导致派生事件中的数据丢失。
//
// 使用 unique_ptr 可以保留完整派生类型。
// ============================================================

class EventQueue
{
public:

    // 添加事件。
    void push(std::unique_ptr<Event> event);

    // 获取队首事件。
    // 返回 nullptr 表示当前没有事件。
    std::unique_ptr<Event> pop();

    // 判断队列是否为空。
    bool empty() const;

    // 获取当前事件数量。
    std::size_t size() const;

private:

    // FIFO 多态事件容器。
    std::queue<std::unique_ptr<Event>> events_;
};

}
