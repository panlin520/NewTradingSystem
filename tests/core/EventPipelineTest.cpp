#include <cassert>
#include <iostream>
#include <memory>

#include "trading/core/EventBus.hpp"
#include "trading/core/EventQueue.hpp"
#include "trading/marketdata/MarketDataEvent.hpp"

using namespace CMETradingSystem;

int main()
{
    Core::EventQueue queue;
    Core::EventBus bus;

    bool received = false;

    bus.subscribe(
        [&](const Core::Event& event)
        {
            assert(event.type == Core::EventType::MARKET_DATA);

            auto& market_event =
                static_cast<const MarketData::MarketDataEvent&>(event);

            assert(market_event.price == 6000250000000);
            assert(market_event.size == 10);
            assert(market_event.order_id == 123456);
            assert(market_event.sequence == 1);

            received = true;
        }
    );

    auto event = std::make_unique<MarketData::MarketDataEvent>();

    event->price = 6000250000000;
    event->size = 10;
    event->order_id = 123456;
    event->sequence = 1;
    event->symbol = "ESU6";

    queue.push(std::move(event));

    auto queued_event = queue.pop();

    assert(queued_event != nullptr);

    bus.publish(*queued_event);

    assert(received);

    std::cout << "EventPipelineTest passed" << std::endl;

    return 0;
}
