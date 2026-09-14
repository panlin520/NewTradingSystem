#include "trading/marketdata/databento/DBNRecordDecoder.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"
#include "trading/marketdata/databento/MBOSchema.hpp"

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

    if (header.rtype == MBOSchema::MBO_RTYPE)
    {
        MBORecord mbo{};

        return decode_mbo(
            data + sizeof(DBNRecordHeader),
            size - sizeof(DBNRecordHeader),
            mbo
        );
    }

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

    // The payload parser is intentionally kept disabled until
    // the Databento rtype=160 schema offsets are verified.
    // This prevents silently decoding incorrect price/order_id data.

    (void)record;

    return false;
}

}
