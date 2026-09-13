#include "trading/marketdata/HistoricalFeed.hpp"

#include <utility>

namespace CMETradingSystem::MarketData {

HistoricalFeed::HistoricalFeed(std::string file_path)
    : file_path_(std::move(file_path))
{
}


HistoricalFeed::~HistoricalFeed() noexcept = default;


FeedStatus HistoricalFeed::next(MarketDataEvent& event)
{
    // 当前阶段只建立统一 Feed 生命周期。
    // 具体历史文件解析将在 Databento MBO Parser 阶段实现。
    (void)event;

    if (end_of_stream_)
    {
        return FeedStatus::END_OF_STREAM;
    }

    return FeedStatus::END_OF_STREAM;
}

}
