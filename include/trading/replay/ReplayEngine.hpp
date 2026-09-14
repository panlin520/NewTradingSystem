#pragma once

#include "trading/marketdata/MarketDataFeed.hpp"
#include "trading/orderbook/OrderBookBuilder.hpp"

namespace CMETradingSystem::Replay
{

class ReplayEngine
{
public:

    ReplayEngine(
        MarketData::MarketDataFeed& feed,
        Trading::OrderBook::OrderBookBuilder& builder
    );

    bool step();

    void run();

    bool finished() const;

private:

    MarketData::MarketDataFeed* feed_{nullptr};

    Trading::OrderBook::OrderBookBuilder* builder_{nullptr};

    bool finished_{false};
};

}
