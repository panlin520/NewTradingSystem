#pragma once

#include "trading/marketdata/MarketDataFeed.hpp"

#include <fstream>
#include <string>

namespace CMETradingSystem::MarketData {

class HistoricalFeed final : public MarketDataFeed
{
public:

    explicit HistoricalFeed(std::string file_path);

    ~HistoricalFeed() noexcept override;

    [[nodiscard]] FeedStatus next(MarketDataEvent& event) override;

private:

    std::string file_path_;

    std::ifstream file_;

    bool end_of_stream_{false};
};

}
