#include <cassert>
#include <iostream>
#include <memory>

#include "trading/core/Dispatcher.hpp"
#include "trading/core/Engine.hpp"
#include "trading/core/EventBus.hpp"
#include "trading/core/EventQueue.hpp"
#include "trading/marketdata/MarketDataEvent.hpp"

using namespace CMETradingSystem;

// ============================================================
// EventPipelineTest
// ============================================================
//
// 目标：
// 验证 MarketDataEvent 能够完整经过生产事件链：
//
// MarketDataEvent
//      |
//      v
// EventQueue
//      |
//      v
// Engine::tick()
//      |
//      v
// Dispatcher
//      |
//      v
// EventBus
//      |
//      v
// Subscriber Handler
//
// 重点验证：
// 1. unique_ptr<Event> 不会造成对象切片。
// 2. EventType 在整个事件链中保持正确。
// 3. MarketDataEvent 的派生字段不会丢失。
// 4. Engine 确实从 EventQueue 消费事件并交给 Dispatcher。
// ============================================================

int main()
{
    Core::EventBus event_bus;
    Core::EventQueue event_queue;
    Core::Dispatcher dispatcher(event_bus);

    Core::Engine engine(
        Core::EngineMode::BACKTEST,
        dispatcher,
        event_queue
    );

    bool received = false;

    // --------------------------------------------------------
    // 注册事件订阅者
    // --------------------------------------------------------
    // EventBus 的统一接口接收 const Core::Event&。
    // 先检查 EventType，再转换成对应的派生事件类型。
    // --------------------------------------------------------
    event_bus.subscribe(
        [&](const Core::Event& event)
        {
            assert(event.type == Core::EventType::MARKET_DATA);

            const auto& market_event =
                static_cast<const MarketData::MarketDataEvent&>(event);

            assert(market_event.ts_event == 1000000000ULL);
            assert(market_event.ts_recv == 1000000100ULL);
            assert(market_event.action == 'A');
            assert(market_event.side == 'B');
            assert(market_event.price == 6000250000000LL);
            assert(market_event.size == 10U);
            assert(market_event.order_id == 123456ULL);
            assert(market_event.sequence == 1ULL);
            assert(market_event.symbol == "ESU6");

            received = true;
        }
    );

    // --------------------------------------------------------
    // 构造一个模拟的 CME MBO MarketDataEvent
    // --------------------------------------------------------
    auto event = std::make_unique<MarketData::MarketDataEvent>();

    event->ts_event = 1000000000ULL;
    event->ts_recv = 1000000100ULL;
    event->action = 'A';
    event->side = 'B';
    event->price = 6000250000000LL;
    event->size = 10U;
    event->order_id = 123456ULL;
    event->sequence = 1ULL;
    event->symbol = "ESU6";

    // MarketDataEvent 通过基类 unique_ptr 进入统一事件队列。
    event_queue.push(std::move(event));

    assert(event_queue.size() == 1U);
    assert(!event_queue.empty());

    // --------------------------------------------------------
    // 启动 Engine，并执行一次 tick
    // --------------------------------------------------------
    // tick() 应完成：
    // EventQueue -> Dispatcher -> EventBus -> Handler
    // --------------------------------------------------------
    engine.start();

    assert(engine.running());

    engine.tick();

    // 事件应已经被 Engine 消费。
    assert(event_queue.empty());
    assert(event_queue.size() == 0U);

    // Handler 必须收到完整 MarketDataEvent。
    assert(received);

    engine.stop();

    assert(!engine.running());

    std::cout << "EventPipelineTest passed" << std::endl;

    return 0;
}
