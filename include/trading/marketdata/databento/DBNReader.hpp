#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "trading/marketdata/databento/DBNRecord.hpp"

namespace CMETradingSystem::MarketData::Databento {

#pragma pack(push,1)

struct DBNHeader
{
    uint8_t version{0};
    uint8_t schema[3]{0,0,0};
    uint8_t encoding[4]{0,0,0,0};
    uint8_t dataset[4]{0,0,0,0};
};

#pragma pack(pop)


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

private:

    std::string file_path_;

    bool opened_{false};

    DBNHeader header_{};

    std::vector<uint8_t> decompressed_data_;

    size_t current_offset_{0};
};

}
