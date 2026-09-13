#pragma once

#include <cstdint>
#include <string>

namespace CMETradingSystem::MarketData::Databento {

// ============================================================
// DBNReader
// ============================================================
//
// Databento DBN / DBN.ZST 文件读取器。
//
// 职责：
// - 打开 Databento 历史数据文件
// - 按原始顺序读取 Record
// - 提供给 DatabentoFeed 使用
//
// 不负责：
// - MarketDataEvent 创建
// - EventQueue
// - Engine
// - OrderBook
//
// 数据流：
//
// ESU6_2026-06-15_MBO.dbn.zst
//          |
//          v
//       DBNReader
//          |
//          v
//   Databento Record
//
// ============================================================
class DBNReader
{
public:

    explicit DBNReader(std::string file_path);

    ~DBNReader() noexcept;

    [[nodiscard]] bool open();

    [[nodiscard]] bool is_open() const noexcept;


private:

    std::string file_path_;

    bool opened_{false};
};

}
