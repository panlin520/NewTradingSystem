#pragma once

#include "trading/core/Event.hpp"

#include <functional>
#include <vector>

namespace CMETradingSystem::Core {

// ============================================================
// EventBus
// ============================================================
//
// EventBus（事件总线）是整个交易系统内部消息传递的中心。
//
// 小白理解：
//
// 想象交易系统里面有很多部门：
//
//   行情模块 MarketData
//          |
//          | 产生消息(Event)
//          v
//      EventBus
//          |
//          +-------------> OrderBook
//          |
//          +-------------> Strategy
//          |
//          +-------------> Risk
//
// 这些模块之间不直接互相调用，而是通过 EventBus 通信。
//
// 好处：
// 1. 模块之间低耦合
// 2. 添加新模块时不用修改旧模块
// 3. 回测、模拟、实盘可以使用同一套事件流程
//
// ============================================================
class EventBus
{
public:

    // Handler 是事件处理函数。
    //
    // 小白理解：
    // 当 EventBus 收到一个事件后，会调用所有登记过的 Handler。
    //
    // 例如：
    // 新成交事件 -> Strategy Handler 收到 -> 判断是否交易
    //
    using Handler = std::function<void(const Event&)>;


    // --------------------------------------------------------
    // subscribe
    // --------------------------------------------------------
    // 注册一个事件监听者。
    //
    // 参数：
    // handler : 一个函数，当事件发生时执行。
    //
    // 示例：
    // eventBus.subscribe([](const Event& e){
    //     处理事件
    // });
    //
    void subscribe(Handler handler);


    // --------------------------------------------------------
    // publish
    // --------------------------------------------------------
    // 发布一个事件。
    //
    // 小白理解：
    // 把消息发送到广播中心，然后通知所有订阅者。
    //
    // 参数：
    // event : 当前发生的事件。
    //
    void publish(const Event& event) const;


private:

    // 保存所有已经注册的事件处理函数。
    //
    // 当 publish() 被调用时，EventBus 会遍历这个列表，
    // 依次通知每一个模块。
    std::vector<Handler> handlers_;
};

}
