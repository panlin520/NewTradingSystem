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
    if (!std::filesystem::exists(file_path_))
    {
        opened_ = false;
        return false;
    }

    opened_ = true;
    return true;
}


bool DBNReader::read_header()
{
    // ============================================================
    // Stage 2 placeholder.
    //
    // Real DBN binary header decoding will be implemented here.
    // Current function only validates that the reader is open.
    // ============================================================

    if (!opened_)
    {
        return false;
    }

    return true;
}


bool DBNReader::is_open() const noexcept
{
    return opened_;
}


const DBNHeader& DBNReader::header() const noexcept
{
    return header_;
}

}
