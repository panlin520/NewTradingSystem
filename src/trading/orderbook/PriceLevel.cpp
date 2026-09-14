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
            if (volume_ >= it->size)
            {
                volume_ -= it->size;
            }
            else
            {
                volume_ = 0;
            }

            orders_.erase(it);
            return true;
        }
    }

    return false;
}

Order* PriceLevel::front()
{
    if (orders_.empty())
    {
        return nullptr;
    }

    return &orders_.front();
}

const Order* PriceLevel::front() const
{
    if (orders_.empty())
    {
        return nullptr;
    }

    return &orders_.front();
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
