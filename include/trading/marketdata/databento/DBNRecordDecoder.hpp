#pragma once

#include "trading/marketdata/databento/DBNRecord.hpp"
#include "trading/marketdata/databento/MBORecord.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// DBNRecordDecoder
// ============================================================
//
// Decodes one complete Databento DBN record from binary bytes.
//
// Important contract:
// - `data` points to byte 0 of the DBN RecordHeader.
// - `size` is the complete record size in bytes.
// - For MBO, `decode_mbo()` also receives the complete record,
//   not only the payload after RecordHeader.
//
// This keeps all binary offsets relative to the official Databento
// RecordHeader / MboMsg layout and avoids duplicate offset systems.
// ============================================================

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
