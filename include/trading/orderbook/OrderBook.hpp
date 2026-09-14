#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include "Order.hpp"
#include "PriceLevel.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

class OrderBook
{
public:
    bool add_order(const Order& order);

    bool cancel_order(uint64_t order_id);

    bool modify_order(uint64_t order_id, uint32_t new_size);

    void clear();

    const Order* find_order(uint64_t order_id) const;

    uint64_t bid_volume() const;

    uint64_t ask_volume() const;

    const PriceLevel* best_bid() const;

    const PriceLevel* best_ask() const;

    size_t order_count() const;

private:
    std::map<int64_t, PriceLevel, std::greater<int64_t>> bids_;
    std::map<int64_t, PriceLevel> asks_;
    std::unordered_map<uint64_t, Order> orders_;
};

}
