#pragma once

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Databento MBO binary layout
// ============================================================
//
// Source of truth:
//   Databento DBN RecordHeader / MboMsg definitions.
//
// Official properties used here:
//
//   RecordHeader size : 16 bytes
//   MboMsg base size  : 56 bytes
//   MBO rtype         : 0xA0 (160)
//   Record length     : stored in 32-bit words
//
// Databento's official layout tests assert MboMsg has no internal
// padding. These offsets therefore describe the binary record from
// byte 0 of the RecordHeader.
//
// A DBN stream can append additional fields such as ts_out to a
// record. BASE_RECORD_SIZE is therefore the minimum MBO size, not
// necessarily the complete record length in every stream.
// ============================================================

struct MBOSchema
{
    static constexpr uint8_t MBO_RTYPE = 0xA0;

    static constexpr size_t WORD_SIZE = 4;
    static constexpr size_t RECORD_HEADER_SIZE = 16;
    static constexpr size_t BASE_RECORD_SIZE = 56;
    static constexpr uint8_t BASE_RECORD_LENGTH_WORDS = 14;

    // RecordHeader offsets.
    static constexpr size_t LENGTH_OFFSET = 0;
    static constexpr size_t RTYPE_OFFSET = 1;
    static constexpr size_t PUBLISHER_ID_OFFSET = 2;
    static constexpr size_t INSTRUMENT_ID_OFFSET = 4;
    static constexpr size_t TS_EVENT_OFFSET = 8;

    // MboMsg offsets from the beginning of the record.
    static constexpr size_t ORDER_ID_OFFSET = 16;
    static constexpr size_t PRICE_OFFSET = 24;
    static constexpr size_t SIZE_OFFSET = 32;
    static constexpr size_t FLAGS_OFFSET = 36;
    static constexpr size_t CHANNEL_ID_OFFSET = 37;
    static constexpr size_t ACTION_OFFSET = 38;
    static constexpr size_t SIDE_OFFSET = 39;
    static constexpr size_t TS_RECV_OFFSET = 40;
    static constexpr size_t TS_IN_DELTA_OFFSET = 48;
    static constexpr size_t SEQUENCE_OFFSET = 52;
};

static_assert(MBOSchema::SEQUENCE_OFFSET + sizeof(uint32_t) ==
              MBOSchema::BASE_RECORD_SIZE);

}
