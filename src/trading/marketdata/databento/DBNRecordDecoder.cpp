#include "trading/marketdata/databento/DBNRecordDecoder.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"

#include <cstring>

namespace CMETradingSystem::MarketData::Databento
{

bool DBNRecordDecoder::read_record_header(
    const uint8_t* data,
    size_t size,
    DBNRecordHeader& header
) const
{
    if (data == nullptr || size < sizeof(DBNRecordHeader))
    {
        return false;
    }

    std::memcpy(
        &header,
        data,
        sizeof(DBNRecordHeader)
    );

    return true;
}


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

    DBNRecordHeader header{};

    if (!read_record_header(data, size, header))
    {
        return false;
    }

    record.rtype = header.rtype;

    return true;
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

    // MBO schema decoding will be added after validating
    // Databento rtype=160 binary layout.
    return false;
}

}
