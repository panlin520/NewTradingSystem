#pragma once

#include <cstddef>
#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Databento MBO schema description
// ============================================================
//
// This file defines the decoder layout abstraction.
//
// The decoder must not assume C++ struct packing matches the
// Databento binary format. Field offsets are kept here so the
// binary parser and validation tests use one source of truth.
//
// Actual offsets are filled after validating the DBN rtype=160
// schema from the dataset header.
// ============================================================

struct MBOSchema
{
    static constexpr uint8_t MBO_RTYPE = 160;

    // Record header bytes are handled separately.
    static constexpr size_t HEADER_SIZE = 0;

    // Payload offsets.
    // TODO: populate from Databento MBO schema validation.
    static constexpr size_t TS_EVENT_OFFSET = 0;
    static constexpr size_t TS_RECV_OFFSET = 0;
    static constexpr size_t ACTION_OFFSET = 0;
    static constexpr size_t SIDE_OFFSET = 0;
    static constexpr size_t PRICE_OFFSET = 0;
    static constexpr size_t SIZE_OFFSET = 0;
    static constexpr size_t ORDER_ID_OFFSET = 0;
    static constexpr size_t SEQUENCE_OFFSET = 0;
};

}
