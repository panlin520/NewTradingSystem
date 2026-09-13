#include "trading/marketdata/HistoricalFeed.hpp"

#include <utility>

namespace CMETradingSystem::MarketData {

HistoricalFeed::HistoricalFeed(std::string file_path)
    : file_path_(std::move(file_path)),
      file_(file_path_)
{
}

HistoricalFeed::~HistoricalFeed() noexcept = default;

FeedStatus HistoricalFeed::next(MarketDataEvent& event)
{
    if (end_of_stream_)
    {
        return FeedStatus::END_OF_STREAM;
    }

    if (!file_.is_open())
    {
        return FeedStatus::ERROR;
    }

    // 当前阶段只验证历史文件生命周期。
    // Databento MBO 解析将在专用 Parser 阶段接入。
    (void)event;

    end_of_stream_ = true;

    return FeedStatus::END_OF_STREAM;
}

}
