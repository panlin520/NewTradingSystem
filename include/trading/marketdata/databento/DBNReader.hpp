#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace CMETradingSystem::MarketData::Databento {

struct DBNHeader
{
    uint8_t version{0};
    uint32_t schema{0};
    uint32_t encoding{0};
    uint32_t dataset{0};
};


// ============================================================
// DBNReader
// ============================================================
//
// Databento DBN / DBN.ZST 文件读取器。
//
// 当前阶段：
// - 打开 Databento 压缩文件
// - zstd 解压
// - 准备 DBN Header 读取
//
// 不负责：
// - MarketDataEvent 创建
// - EventQueue
// - Engine
// - OrderBook
//
// ============================================================
class DBNReader
{
public:

    explicit DBNReader(std::string file_path);

    ~DBNReader() noexcept;

    [[nodiscard]] bool open();

    [[nodiscard]] bool read_header();

    [[nodiscard]] bool is_open() const noexcept;

    [[nodiscard]] const DBNHeader& header() const noexcept;

private:

    std::string file_path_;

    bool opened_{false};

    DBNHeader header_{};

    std::vector<uint8_t> decompressed_data_;
};

}
