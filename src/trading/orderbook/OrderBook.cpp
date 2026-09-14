#include "trading/orderbook/OrderBook.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

bool OrderBook::add_order(const Order& order)
{
    if (order.order_id == 0)
        return false;

    if (orders_.contains(order.order_id))
        return false;

    orders_[order.order_id] = order;

    if (order.side == OrderSide::BUY)
    {
        auto [it, inserted] = bids_.try_emplace(order.price, PriceLevel(order.price));
        it->second.add(order);
    }
    else if (order.side == OrderSide::SELL)
    {
        auto [it, inserted] = asks_.try_emplace(order.price, PriceLevel(order.price));
        it->second.add(order);
    }
    else
    {
        orders_.erase(order.order_id);
        return false;
    }

    return true;
}

bool OrderBook::cancel_order(uint64_t order_id)
{
    auto it = orders_.find(order_id);
    if (it == orders_.end())
        return false;

    auto& order = it->second;

    if (order.side == OrderSide::BUY)
    {
        auto level = bids_.find(order.price);
        if (level != bids_.end())
            level->second.remove(order_id);
    }
    else if (order.side == OrderSide::SELL)
    {
        auto level = asks_.find(order.price);
        if (level != asks_.end())
            level->second.remove(order_id);
    }

    orders_.erase(it);
    return true;
}

bool OrderBook::modify_order(uint64_t order_id, uint32_t new_size)
{
    auto it = orders_.find(order_id);
    if (it == orders_.end())
        return false;

    auto& order = it->second;

    if (order.side == OrderSide::BUY)
    {
        auto level = bids_.find(order.price);
        if (level == bids_.end())
            return false;

        level->second.modify(order_id, new_size);
    }
    else if (order.side == OrderSide::SELL)
    {
        auto level = asks_.find(order.price);
        if (level == asks_.end())
            return false;

        level->second.modify(order_id, new_size);
    }

    order.size = new_size;
    return true;
}

void OrderBook::clear()
{
    bids_.clear();
    asks_.clear();
    orders_.clear();
}

const Order* OrderBook::find_order(uint64_t order_id) const
{
    auto it = orders_.find(order_id);
    return it == orders_.end() ? nullptr : &it->second;
}

uint64_t OrderBook::bid_volume() const
{
    uint64_t total = 0;
    for (const auto& [price, level] : bids_)
        total += level.volume();
    return total;
}

uint64_t OrderBook::ask_volume() const
{
    uint64_t total = 0;
    for (const auto& [price, level] : asks_)
        total += level.volume();
    return total;
}

const PriceLevel* OrderBook::best_bid() const
{
    return bids_.empty() ? nullptr : &bids_.begin()->second;
}

const PriceLevel* OrderBook::best_ask() const
{
    return asks_.empty() ? nullptr : &asks_.begin()->second;
}

size_t OrderBook::order_count() const
{
    return orders_.size();
}

}
