#pragma once

#include <cstdint>
#include <string>

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// Databento Market By Order intermediate record
// ============================================================
//
// Binary MBO data is decoded into this structure before being
// copied into the generic DBNRecord representation.
//
// Types match the official Databento MBO schema.
// Symbol is not physically stored in each MboMsg and therefore
// remains empty until symbology resolution is added.
// ============================================================

struct MBORecord
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
