#pragma once

#include <cstdint>
#include <deque>
#include "Order.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

// ============================================================
// L3 Price Level
// ============================================================
//
// Maintains orders at the same price.
//
// CME MBO priority rule:
// same price -> FIFO by arrival sequence.
//
// Responsibilities:
// - store order queue
// - maintain aggregated volume
// - remove orders by order_id
//
// ============================================================

class PriceLevel
{
public:
    explicit PriceLevel(int64_t price);

    void add(Order order);

    bool remove(uint64_t order_id);

    Order* front();

    const Order* front() const;

    int64_t price() const;

    uint64_t volume() const;

    bool empty() const;

    size_t order_count() const;

private:
    int64_t price_{0};

    std::deque<Order> orders_;

    uint64_t volume_{0};
};

}
