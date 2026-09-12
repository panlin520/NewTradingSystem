#pragma once

#include <cstdint>
#include <string>

#include "trading/core/Event.hpp"

namespace CMETradingSystem::MarketData {

// ============================================================
// MarketDataEvent
// ============================================================
//
// CME MDP 3.0 MBO Level 3 market data event.
//
// Every order book change from Databento MBO will eventually be
// converted into this event type before entering the Core event
// pipeline.
//
// Flow:
//
// Databento MBO
//      |
//      v
// MarketDataEvent
//      |
//      v
// Core::EventQueue
//      |
//      v
// Engine -> Dispatcher -> EventBus
//
// ============================================================

struct MarketDataEvent : public Core::Event
{
    MarketDataEvent()
    {
        type = Core::EventType::MARKET_DATA;
    }

    // Exchange event timestamp (nanoseconds).
    uint64_t ts_event{0};

    // Receive timestamp (nanoseconds).
    uint64_t ts_recv{0};

    // Databento MBO action:
    // A = Add
    // M = Modify
    // C = Cancel
    // R = Reset
    // T = Trade
    // F = Fill
    // N = Notification
    char action{' '};

    // Order side:
    // B = Bid
    // A = Ask
    char side{' '};

    // CME price in fixed integer format.
    int64_t price{0};

    // Order quantity.
    uint32_t size{0};

    // Individual order identifier.
    uint64_t order_id{0};

    // Exchange sequence number.
    uint64_t sequence{0};

    // Trading symbol, for example ESU6.
    std::string symbol;
};

}
