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

    // Databento MBO binary layout decoding will be implemented here.
    // Current stage only validates input buffer.

    return false;
}

}
