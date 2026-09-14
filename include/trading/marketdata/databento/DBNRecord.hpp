#pragma once

#include <cstdint>
#include <string>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Internal Databento DBN record representation
// ============================================================
//
// This is the normalized structure produced by DBNRecordDecoder
// before the data is converted into MarketDataEvent.
//
// Field types intentionally follow Databento's official MBO schema.
// Symbol is kept separately because it is not embedded in each MBO
// binary record; it is resolved from DBN symbology metadata/mappings.
// ============================================================

struct DBNRecord
{
    uint64_t ts_event{0};
    uint64_t ts_recv{0};

    uint8_t rtype{0};
    uint16_t publisher_id{0};
    uint32_t instrument_id{0};

    char action{0};
    char side{0};

    int64_t price{0};
    uint32_t size{0};

    uint8_t channel_id{0};
    uint64_t order_id{0};

    uint8_t flags{0};
    int32_t ts_in_delta{0};

    uint32_t sequence{0};

    std::string symbol;
};

}
