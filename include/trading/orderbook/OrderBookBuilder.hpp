#pragma once

#include "OrderBook.hpp"
#include "Order.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

class OrderBookBuilder
{
public:
    bool process(
        char action,
        char side,
        uint64_t order_id,
        int64_t price,
        uint32_t size,
        uint64_t ts_event,
        uint32_t sequence);

    OrderBook& book();
    const OrderBook& book() const;

private:
    OrderBook book_;
};

}
