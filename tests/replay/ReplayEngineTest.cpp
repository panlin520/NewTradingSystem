#include <gtest/gtest.h>

#include <string>

#include "trading/marketdata/DatabentoFeed.hpp"
#include "trading/replay/ReplayEngine.hpp"

namespace
{

std::string TestDBNPath()
{
    return "data/ESU6_2026-06-15_MBO.dbn.zst";
}

class MockFeed : public CMETradingSystem::MarketData::MarketDataFeed
{
public:
    CMETradingSystem::MarketData::FeedStatus next(CMETradingSystem::MarketData::MarketDataEvent& event) override
    {
        if (sent_) return CMETradingSystem::MarketData::FeedStatus::END_OF_STREAM;
        event.action='A'; event.side='B'; event.order_id=1; event.price=5000; event.size=10; event.ts_event=1; event.sequence=1;
        sent_=true;
        return CMETradingSystem::MarketData::FeedStatus::EVENT;
    }
private:
    bool sent_{false};
};

class Replay100KMockFeed : public CMETradingSystem::MarketData::MarketDataFeed
{
public:
    CMETradingSystem::MarketData::FeedStatus next(CMETradingSystem::MarketData::MarketDataEvent& event) override
    {
        if (count_ >= 100000) return CMETradingSystem::MarketData::FeedStatus::END_OF_STREAM;
        event.action='A'; event.side=(count_%2==0)?'B':'A'; event.order_id=count_+1; event.price=5000; event.size=1; event.ts_event=count_; event.sequence=count_;
        ++count_;
        return CMETradingSystem::MarketData::FeedStatus::EVENT;
    }
private:
    uint64_t count_{0};
};

}

TEST(ReplayEngineTest, ReplayOneMarketEvent)
{
    MockFeed feed;
    CMETradingSystem::Trading::OrderBook::OrderBookBuilder builder;
    CMETradingSystem::Replay::ReplayEngine replay(feed,builder);

    EXPECT_TRUE(replay.step());
    EXPECT_FALSE(replay.finished());
    EXPECT_EQ(builder.book().order_count(),1);

    EXPECT_FALSE(replay.step());
    EXPECT_TRUE(replay.finished());
}

TEST(ReplayEngineTest, Replay100KMarketEvents)
{
    Replay100KMockFeed feed;
    CMETradingSystem::Trading::OrderBook::OrderBookBuilder builder;
    CMETradingSystem::Replay::ReplayEngine replay(feed,builder);

    replay.run();

    EXPECT_TRUE(replay.finished());
    EXPECT_EQ(builder.book().order_count(),100000);
}

TEST(ReplayEngineTest, ReplayRealESU6MBO100KEvents)
{
    CMETradingSystem::MarketData::DatabentoFeed feed(TestDBNPath());
    CMETradingSystem::Trading::OrderBook::OrderBookBuilder builder;
    CMETradingSystem::Replay::ReplayEngine replay(feed,builder);

    size_t event_count = 0;
    constexpr size_t TARGET_EVENTS = 100000;

    while(event_count < TARGET_EVENTS && replay.step())
    {
        ++event_count;
    }

    EXPECT_EQ(event_count,TARGET_EVENTS);
    EXPECT_GT(builder.book().order_count(),0);
}
