#include "trading/replay/ReplayEngine.hpp"

namespace CMETradingSystem::Replay
{

ReplayEngine::ReplayEngine(
    MarketData::MarketDataFeed& feed,
    Trading::OrderBook::OrderBookBuilder& builder
)
    : feed_(&feed),
      builder_(&builder)
{
}

bool ReplayEngine::step()
{
    if (finished_)
    {
        return false;
    }

    MarketData::MarketDataEvent event;

    const auto status = feed_->next(event);

    switch (status)
    {
        case MarketData::FeedStatus::EVENT:
        {
            builder_->process(
                event.action,
                event.side,
                event.order_id,
                event.price,
                event.size,
                event.ts_event,
                static_cast<uint32_t>(event.sequence)
            );

            return true;
        }

        case MarketData::FeedStatus::END_OF_STREAM:
        case MarketData::FeedStatus::ERROR:
        {
            finished_ = true;
            return false;
        }

        case MarketData::FeedStatus::IDLE:
        default:
            return false;
    }
}

void ReplayEngine::run()
{
    while (step())
    {
    }
}

bool ReplayEngine::finished() const
{
    return finished_;
}

}
