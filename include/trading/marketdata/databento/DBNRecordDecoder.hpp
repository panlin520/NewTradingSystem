#pragma once

#include "trading/marketdata/databento/DBNRecord.hpp"
#include "trading/marketdata/databento/MBORecord.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

class DBNRecordDecoder
{
public:

    DBNRecordDecoder() = default;

    bool decode(
        const uint8_t* data,
        size_t size,
        DBNRecord& record
    ) const;

    bool decode_mbo(
        const uint8_t* data,
        size_t size,
        MBORecord& record
    ) const;

private:

    bool read_record_header(
        const uint8_t* data,
        size_t size,
        DBNRecordHeader& header
    ) const;

};

}
