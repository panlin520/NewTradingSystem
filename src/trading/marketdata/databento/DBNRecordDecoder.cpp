#include "trading/marketdata/databento/DBNRecordDecoder.hpp"

namespace CMETradingSystem::MarketData::Databento
{

bool DBNRecordDecoder::decode(
    const uint8_t* data,
    size_t size,
    DBNRecord& record
) const
{
    if (data == nullptr || size == 0)
    {
        return false;
    }

    return false;
}


bool DBNRecordDecoder::decode_mbo(
    const uint8_t* data,
    size_t size,
    MBORecord& record
) const
{
    if (data == nullptr || size == 0)
    {
        return false;
    }

    // Databento MBO binary decoding will be implemented after
    // validating the exact DBN schema layout and record size.
    // Do not perform raw memcpy here because field offsets are schema-defined.

    return false;
}

}
