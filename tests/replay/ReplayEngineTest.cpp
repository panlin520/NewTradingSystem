#include <gtest/gtest.h>

#include "trading/replay/ReplayEngine.hpp"

namespace
{

class MockFeed : public CMETradingSystem::MarketData::MarketDataFeed
{
public:

    CMETradingSystem::MarketData::FeedStatus next(
        CMETradingSystem::MarketData::MarketDataEvent& event
    ) override
    {
        if (sent_)
        {
            return CMETradingSystem::MarketData::FeedStatus::END_OF_STREAM;
        }

        event.action = 'A';
        event.side = 'B';
        event.order_id = 1;
        event.price = 5000;
        event.size = 10;
        event.ts_event = 1;
        event.sequence = 1;

        sent_ = true;

        return CMETradingSystem::MarketData::FeedStatus::EVENT;
    }

private:
    bool sent_{false};
};

}

TEST(ReplayEngineTest, ReplayOneMarketEvent)
{
    MockFeed feed;

    CMETradingSystem::Trading::OrderBook::OrderBookBuilder builder;

    CMETradingSystem::Replay::ReplayEngine replay(
        feed,
        builder
    );

    EXPECT_TRUE(replay.step());
    EXPECT_FALSE(replay.finished());

    EXPECT_EQ(
        builder.book().order_count(),
        1
    );

    EXPECT_FALSE(replay.step());
    EXPECT_TRUE(replay.finished());
}
