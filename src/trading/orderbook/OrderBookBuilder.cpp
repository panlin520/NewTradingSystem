#include "trading/orderbook/OrderBookBuilder.hpp"

namespace CMETradingSystem::Trading::OrderBook
{

bool OrderBookBuilder::process(
    char action,
    char side,
    uint64_t order_id,
    int64_t price,
    uint32_t size,
    uint64_t ts_event,
    uint32_t sequence)
{
    switch (action)
    {
    case 'A':
    {
        Order order;
        order.order_id = order_id;
        order.price = price;
        order.size = size;
        order.side = side == 'B' ? OrderSide::BUY : OrderSide::SELL;
        order.ts_event = ts_event;
        order.sequence = sequence;

        return book_.add_order(order);
    }

    case 'M':
        return book_.modify_order(order_id, size);

    case 'C':
    case 'F':
        return book_.cancel_order(order_id);

    case 'R':
        book_.clear();
        return true;

    case 'T':
    case 'N':
        return true;

    default:
        return false;
    }
}

OrderBook& OrderBookBuilder::book()
{
    return book_;
}

const OrderBook& OrderBookBuilder::book() const
{
    return book_;
}

}
