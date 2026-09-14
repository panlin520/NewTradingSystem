#pragma once

#include "trading/marketdata/MarketDataFeed.hpp"
#include "trading/marketdata/databento/DBNReader.hpp"

#include <string>

namespace CMETradingSystem::MarketData {

// ============================================================
// DatabentoFeed
// ============================================================
//
// Databento historical CME MBO feed implementation.
//
// Responsibility:
// - Open a .dbn.zst file through Databento::DBNReader.
// - Read records in their original DBN order.
// - Convert MBO DBNRecord values into the unified MarketDataEvent.
// - Report EVENT / END_OF_STREAM / ERROR through MarketDataFeed.
//
// Not responsible for:
// - Replay speed / pause / seek control
// - OrderBook reconstruction
// - Feature calculation
// - Strategy
// - Risk
// - Execution
//
// Replay control belongs to the later Replay Engine. This class only
// provides the deterministic historical event stream required by it.
// ============================================================
class DatabentoFeed final : public MarketDataFeed
{
public:

    explicit DatabentoFeed(std::string file_path);

    ~DatabentoFeed() noexcept override;

    [[nodiscard]] FeedStatus next(MarketDataEvent& event) override;

private:

    [[nodiscard]] bool initialize();

    Databento::DBNReader reader_;

    bool initialized_{false};

    bool end_of_stream_{false};

    bool failed_{false};
};

}
