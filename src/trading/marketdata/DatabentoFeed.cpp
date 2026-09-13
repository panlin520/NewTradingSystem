#include "trading/marketdata/DatabentoFeed.hpp"

#include <utility>

namespace CMETradingSystem::MarketData {

DatabentoFeed::DatabentoFeed(std::string file_path)
    : file_path_(std::move(file_path))
{
}


DatabentoFeed::~DatabentoFeed() noexcept = default;


FeedStatus DatabentoFeed::next(MarketDataEvent& event)
{
    // DBN / DBN.ZST 解析将在下一阶段实现。
    // 当前只保留统一 Feed 接口。
    (void)event;

    return FeedStatus::END_OF_STREAM;
}

}
