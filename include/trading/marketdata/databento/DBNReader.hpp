#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "trading/marketdata/databento/DBNRecord.hpp"

namespace CMETradingSystem::MarketData::Databento
{

// ============================================================
// DBN file prelude
// ============================================================
//
// A DBN file starts with:
//
//   bytes 0..2 : ASCII "DBN"
//   byte  3    : DBN encoding version
//   bytes 4..7 : metadata length (little-endian uint32_t)
//   bytes 8..  : metadata payload
//
// The metadata payload length does not include this 8-byte prelude.
// Records begin immediately after the metadata payload.
//
// This structure intentionally models only the verified file prelude.
// Schema, dataset, symbol mappings, and the remaining metadata fields
// will be decoded separately from the metadata payload rather than
// guessed into a fixed C++ struct.
// ============================================================

struct DBNHeader
{
    char magic[3]{'\0', '\0', '\0'};
    uint8_t version{0};
    uint32_t metadata_length{0};
};


class DBNReader
{
public:

    explicit DBNReader(std::string file_path);

    ~DBNReader() noexcept;

    [[nodiscard]] bool open();

    [[nodiscard]] bool read_header();

    [[nodiscard]] bool next_record(DBNRecord& record);

    [[nodiscard]] bool is_open() const noexcept;

    [[nodiscard]] const DBNHeader& header() const noexcept;

    // Byte offset of the first DBN record after the metadata block.
    [[nodiscard]] size_t records_offset() const noexcept;

private:

    static constexpr size_t DBN_PRELUDE_SIZE = 8;

    std::string file_path_;

    bool opened_{false};

    DBNHeader header_{};

    std::vector<uint8_t> decompressed_data_;

    size_t current_offset_{0};

    size_t records_offset_{0};
};

}
