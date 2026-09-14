#pragma once

#include <cstdint>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Databento DBN record header
// ============================================================
//
// The record header is separated from the payload so the decoder
// can determine the record boundary before decoding schema fields.
//
// The exact payload layout is handled by the schema decoder.
// ============================================================

struct DBNRecordHeader
{
    uint16_t length{0};
    uint8_t rtype{0};
};

}
