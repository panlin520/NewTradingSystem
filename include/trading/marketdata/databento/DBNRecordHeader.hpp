#pragma once

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Databento DBN RecordHeader
// ============================================================
//
// Official DBN records always start with this 16-byte header.
//
// Databento definition:
//
//   length        uint8_t   record length in 32-bit words
//   rtype         uint8_t   record type
//   publisher_id  uint16_t  Databento publisher ID
//   instrument_id uint32_t  numeric instrument ID
//   ts_event      uint64_t  event timestamp in UNIX nanoseconds
//
// Important:
//   length is NOT a byte count.
//   Actual record bytes = length * 4.
//
// Keep this structure layout-compatible with the official DBN
// RecordHeader. Do not add fields or change field types here.
// ============================================================

struct DBNRecordHeader
{
    uint8_t length{0};
    uint8_t rtype{0};
    uint16_t publisher_id{0};
    uint32_t instrument_id{0};
    uint64_t ts_event{0};
};

static_assert(sizeof(DBNRecordHeader) == 16,
              "DBNRecordHeader must match Databento's 16-byte RecordHeader");
static_assert(offsetof(DBNRecordHeader, length) == 0);
static_assert(offsetof(DBNRecordHeader, rtype) == 1);
static_assert(offsetof(DBNRecordHeader, publisher_id) == 2);
static_assert(offsetof(DBNRecordHeader, instrument_id) == 4);
static_assert(offsetof(DBNRecordHeader, ts_event) == 8);

}
