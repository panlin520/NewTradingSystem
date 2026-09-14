#include "trading/marketdata/DatabentoFeed.hpp"
#include "trading/marketdata/databento/DBNRecord.hpp"
#include "trading/marketdata/databento/MBOSchema.hpp"

#include <utility>

namespace CMETradingSystem::MarketData {

namespace
{

void copy_record_to_event(
    const Databento::DBNRecord& record,
    MarketDataEvent& event
)
{
    MarketDataEvent parsed{};

    // Core::Event::timestamp represents the event occurrence time.
    // For MarketDataEvent that source-of-truth timestamp is ts_event.
    parsed.timestamp = record.ts_event;

    parsed.ts_event = record.ts_event;
    parsed.ts_recv = record.ts_recv;
    parsed.rtype = record.rtype;
    parsed.publisher_id = record.publisher_id;
    parsed.instrument_id = record.instrument_id;
    parsed.channel_id = record.channel_id;
    parsed.flags = record.flags;
    parsed.ts_in_delta = record.ts_in_delta;
    parsed.action = record.action;
    parsed.side = record.side;
    parsed.price = record.price;
    parsed.size = record.size;
    parsed.order_id = record.order_id;
    parsed.sequence = record.sequence;
    parsed.symbol = record.symbol;

    event = std::move(parsed);
}

}

DatabentoFeed::DatabentoFeed(std::string file_path)
    : reader_(std::move(file_path))
{
}

DatabentoFeed::~DatabentoFeed() noexcept = default;

bool DatabentoFeed::initialize()
{
    if (initialized_)
    {
        return true;
    }

    if (failed_)
    {
        return false;
    }

    if (!reader_.open())
    {
        failed_ = true;
        return false;
    }

    if (!reader_.read_header())
    {
        failed_ = true;
        return false;
    }

    initialized_ = true;
    return true;
}

FeedStatus DatabentoFeed::next(MarketDataEvent& event)
{
    if (end_of_stream_)
    {
        return FeedStatus::END_OF_STREAM;
    }

    if (failed_)
    {
        return FeedStatus::ERROR;
    }

    if (!initialize())
    {
        return FeedStatus::ERROR;
    }

    Databento::DBNRecord record{};

    while (true)
    {
        if (!reader_.next_record(record))
        {
            if (reader_.at_end())
            {
                end_of_stream_ = true;
                return FeedStatus::END_OF_STREAM;
            }

            failed_ = true;
            return FeedStatus::ERROR;
        }

        // Databento DBN streams can contain different rtypes. The current
        // trading pipeline consumes MBO records only, so unrelated records
        // are skipped without changing the relative order of MBO events.
        if (record.rtype != Databento::MBOSchema::MBO_RTYPE)
        {
            continue;
        }

        copy_record_to_event(record, event);
        return FeedStatus::EVENT;
    }
}

}
