#pragma once

#include "trading/marketdata/databento/DBNRecord.hpp"

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

};

}
