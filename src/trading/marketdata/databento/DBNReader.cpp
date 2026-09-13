#include "trading/marketdata/databento/DBNReader.hpp"

#include <utility>

namespace CMETradingSystem::MarketData::Databento {

DBNReader::DBNReader(std::string file_path)
    : file_path_(std::move(file_path))
{
}


DBNReader::~DBNReader() noexcept = default;


bool DBNReader::open()
{
    // 当前阶段只建立 DBN Reader 生命周期。
    // 实际 DBN.ZST 解码将在下一阶段接入 Databento SDK 或解析层。
    opened_ = true;
    return true;
}


bool DBNReader::is_open() const noexcept
{
    return opened_;
}

}
