#pragma once

#include <cstdint>
#include <deque>
#include "Order.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

class PriceLevel
{
public:
    explicit PriceLevel(int64_t price);

    void add(Order order);

    bool remove(uint64_t order_id);

    bool modify(uint64_t order_id, uint32_t new_size);

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
