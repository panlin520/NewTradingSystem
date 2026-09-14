#pragma once

#include "trading/marketdata/databento/DBNRecord.hpp"
#include "trading/marketdata/databento/MBORecord.hpp"

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

class DBNRecordDecoder
{
public:

    DBNRecordDecoder() = default;

    // Decode generic DBN record into internal DBNRecord.
    bool decode(
        const uint8_t* data,
        size_t size,
        DBNRecord& record
    ) const;

    // Decode Databento MBO record.
    // The implementation will follow Databento DBN schema layout.
    bool decode_mbo(
        const uint8_t* data,
        size_t size,
        MBORecord& record
    ) const;

};

}
