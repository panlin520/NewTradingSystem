#include "trading/marketdata/databento/DBNRecordDecoder.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"
#include "trading/marketdata/databento/MBOSchema.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace CMETradingSystem::MarketData::Databento
{

namespace
{

uint16_t read_u16_le(const uint8_t* data) noexcept
{
    return static_cast<uint16_t>(data[0]) |
           static_cast<uint16_t>(static_cast<uint16_t>(data[1]) << 8U);
}

uint32_t read_u32_le(const uint8_t* data) noexcept
{
    return static_cast<uint32_t>(data[0]) |
           (static_cast<uint32_t>(data[1]) << 8U) |
           (static_cast<uint32_t>(data[2]) << 16U) |
           (static_cast<uint32_t>(data[3]) << 24U);
}

uint64_t read_u64_le(const uint8_t* data) noexcept
{
    return static_cast<uint64_t>(data[0]) |
           (static_cast<uint64_t>(data[1]) << 8U) |
           (static_cast<uint64_t>(data[2]) << 16U) |
           (static_cast<uint64_t>(data[3]) << 24U) |
           (static_cast<uint64_t>(data[4]) << 32U) |
           (static_cast<uint64_t>(data[5]) << 40U) |
           (static_cast<uint64_t>(data[6]) << 48U) |
           (static_cast<uint64_t>(data[7]) << 56U);
}

int32_t read_i32_le(const uint8_t* data) noexcept
{
    return std::bit_cast<int32_t>(read_u32_le(data));
}

int64_t read_i64_le(const uint8_t* data) noexcept
{
    return std::bit_cast<int64_t>(read_u64_le(data));
}

}

bool DBNRecordDecoder::read_record_header(
    const uint8_t* data,
    size_t size,
    DBNRecordHeader& header
) const
{
    if (data == nullptr || size < MBOSchema::RECORD_HEADER_SIZE)
    {
        return false;
    }

    header.length = data[MBOSchema::LENGTH_OFFSET];
    header.rtype = data[MBOSchema::RTYPE_OFFSET];
    header.publisher_id = read_u16_le(data + MBOSchema::PUBLISHER_ID_OFFSET);
    header.instrument_id = read_u32_le(data + MBOSchema::INSTRUMENT_ID_OFFSET);
    header.ts_event = read_u64_le(data + MBOSchema::TS_EVENT_OFFSET);

    return true;
}

bool DBNRecordDecoder::decode(
    const uint8_t* data,
    size_t size,
    DBNRecord& record
) const
{
    if (data == nullptr || size < MBOSchema::RECORD_HEADER_SIZE)
    {
        return false;
    }

    DBNRecordHeader header{};

    if (!read_record_header(data, size, header))
    {
        return false;
    }

    const size_t encoded_record_size =
        static_cast<size_t>(header.length) * MBOSchema::WORD_SIZE;

    if (header.length == 0 ||
        encoded_record_size < MBOSchema::RECORD_HEADER_SIZE ||
        encoded_record_size > size)
    {
        return false;
    }

    record = {};
    record.rtype = header.rtype;
    record.publisher_id = header.publisher_id;
    record.instrument_id = header.instrument_id;
    record.ts_event = header.ts_event;

    if (header.rtype != MBOSchema::MBO_RTYPE)
    {
        return true;
    }

    MBORecord mbo{};

    if (!decode_mbo(data, encoded_record_size, mbo))
    {
        return false;
    }

    record.ts_event = mbo.ts_event;
    record.ts_recv = mbo.ts_recv;
    record.rtype = mbo.rtype;
    record.publisher_id = mbo.publisher_id;
    record.instrument_id = mbo.instrument_id;
    record.action = mbo.action;
    record.side = mbo.side;
    record.price = mbo.price;
    record.size = mbo.size;
    record.channel_id = mbo.channel_id;
    record.order_id = mbo.order_id;
    record.flags = mbo.flags;
    record.ts_in_delta = mbo.ts_in_delta;
    record.sequence = mbo.sequence;
    record.symbol = mbo.symbol;

    return true;
}

bool DBNRecordDecoder::decode_mbo(
    const uint8_t* data,
    size_t size,
    MBORecord& record
) const
{
    if (data == nullptr || size < MBOSchema::BASE_RECORD_SIZE)
    {
        return false;
    }

    DBNRecordHeader header{};

    if (!read_record_header(data, size, header))
    {
        return false;
    }

    if (header.rtype != MBOSchema::MBO_RTYPE)
    {
        return false;
    }

    const size_t encoded_record_size =
        static_cast<size_t>(header.length) * MBOSchema::WORD_SIZE;

    // The official Databento base MboMsg is 56 bytes. Records may be
    // larger when an optional field such as ts_out is appended, but
    // they must never be shorter than the base MboMsg layout.
    if (encoded_record_size < MBOSchema::BASE_RECORD_SIZE ||
        encoded_record_size > size)
    {
        return false;
    }

    MBORecord parsed{};

    parsed.ts_event = header.ts_event;
    parsed.ts_recv = read_u64_le(data + MBOSchema::TS_RECV_OFFSET);
    parsed.rtype = header.rtype;
    parsed.publisher_id = header.publisher_id;
    parsed.instrument_id = header.instrument_id;
    parsed.action = static_cast<char>(data[MBOSchema::ACTION_OFFSET]);
    parsed.side = static_cast<char>(data[MBOSchema::SIDE_OFFSET]);
    parsed.price = read_i64_le(data + MBOSchema::PRICE_OFFSET);
    parsed.size = read_u32_le(data + MBOSchema::SIZE_OFFSET);
    parsed.channel_id = data[MBOSchema::CHANNEL_ID_OFFSET];
    parsed.order_id = read_u64_le(data + MBOSchema::ORDER_ID_OFFSET);
    parsed.flags = data[MBOSchema::FLAGS_OFFSET];
    parsed.ts_in_delta = read_i32_le(data + MBOSchema::TS_IN_DELTA_OFFSET);
    parsed.sequence = read_u32_le(data + MBOSchema::SEQUENCE_OFFSET);

    // Symbol is not part of the fixed MboMsg binary structure. It is
    // resolved later from DBN symbology metadata / mapping records.
    parsed.symbol.clear();

    record = std::move(parsed);

    return true;
}

}
