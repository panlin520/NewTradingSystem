#include "trading/marketdata/databento/DBNReader.hpp"

#include <filesystem>
#include <utility>

namespace CMETradingSystem::MarketData::Databento {

DBNReader::DBNReader(std::string file_path)
    : file_path_(std::move(file_path))
{
}


DBNReader::~DBNReader() noexcept = default;


bool DBNReader::open()
{
    // ============================================================
    // Stage 1:
    // Validate DBN file existence.
    //
    // This stage does not decode DBN/ZSTD records yet.
    // It only guarantees that the configured market data file exists.
    // ============================================================

    if (!std::filesystem::exists(file_path_))
    {
        opened_ = false;
        return false;
    }

    opened_ = true;
    return true;
}


bool DBNReader::is_open() const noexcept
{
    return opened_;
}

}
