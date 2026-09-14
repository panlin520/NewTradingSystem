#include "trading/orderbook/PriceLevel.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

PriceLevel::PriceLevel(int64_t price)
    : price_(price)
{
}

void PriceLevel::add(Order order)
{
    volume_ += order.size;
    orders_.push_back(order);
}

bool PriceLevel::remove(uint64_t order_id)
{
    for (auto it = orders_.begin(); it != orders_.end(); ++it)
    {
        if (it->order_id == order_id)
        {
            volume_ -= it->size;
            orders_.erase(it);
            return true;
        }
    }

    return false;
}

bool PriceLevel::modify(uint64_t order_id, uint32_t new_size)
{
    for (auto& order : orders_)
    {
        if (order.order_id == order_id)
        {
            if (new_size >= order.size)
                volume_ += new_size - order.size;
            else
                volume_ -= order.size - new_size;

            order.size = new_size;
            return true;
        }
    }

    return false;
}

Order* PriceLevel::front()
{
    return orders_.empty() ? nullptr : &orders_.front();
}

const Order* PriceLevel::front() const
{
    return orders_.empty() ? nullptr : &orders_.front();
}

int64_t PriceLevel::price() const
{
    return price_;
}

uint64_t PriceLevel::volume() const
{
    return volume_;
}

bool PriceLevel::empty() const
{
    return orders_.empty();
}

size_t PriceLevel::order_count() const
{
    return orders_.size();
}

}
