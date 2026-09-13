#include "trading/marketdata/FeedAdapter.hpp"

namespace CMETradingSystem::MarketData {

FeedAdapter::FeedAdapter(
    MarketDataFeed& feed,
    Core::EventQueue& queue
)
    : feed_(&feed),
      queue_(&queue)
{
}


bool FeedAdapter::poll()
{
    if (feed_ == nullptr || queue_ == nullptr)
    {
        return false;
    }

    MarketDataEvent event;

    const auto status = feed_->next(event);

    if (status != FeedStatus::EVENT)
    {
        return false;
    }

    queue_->push(
        std::make_unique<MarketDataEvent>(event)
    );

    return true;
}

}
