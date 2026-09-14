#pragma once

#include <cstdint>

namespace CMETradingSystem::Trading::OrderBook
{

// ============================================================
// L3 Order
// ============================================================
//
// Represents one individual order from Databento MBO.
// OrderBook stores orders by order_id.
//
// ============================================================

enum class OrderSide : uint8_t
{
    NONE = 0,
    BUY,
    SELL
};

enum class OrderStatus : uint8_t
{
    ACTIVE = 0,
    FILLED,
    CANCELLED
};

struct Order
{
    uint64_t order_id{0};
    int64_t price{0};
    uint32_t size{0};

    OrderSide side{OrderSide::NONE};
    OrderStatus status{OrderStatus::ACTIVE};

    uint64_t ts_event{0};
    uint32_t sequence{0};
};

}
