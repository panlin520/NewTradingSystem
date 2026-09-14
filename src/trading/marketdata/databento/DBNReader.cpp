#include "trading/marketdata/databento/DBNReader.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

#include <zstd.h>

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
        std::cout << "[DBNReader] file not found: " << file_path_ << std::endl;
        opened_ = false;
        return false;
    }

    std::ifstream file(file_path_, std::ios::binary | std::ios::ate);

    if (!file)
    {
        std::cout << "[DBNReader] cannot open file" << std::endl;
        opened_ = false;
        return false;
    }

    const auto compressed_size = static_cast<size_t>(file.tellg());

    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> compressed(compressed_size);

    file.read(
        reinterpret_cast<char*>(compressed.data()),
        compressed_size
    );

    if (!file)
    {
        std::cout << "[DBNReader] file read failed" << std::endl;
        opened_ = false;
        return false;
    }

    const unsigned long long decompressed_size =
        ZSTD_getFrameContentSize(
            compressed.data(),
            compressed.size()
        );

    if (decompressed_size == ZSTD_CONTENTSIZE_ERROR)
    {
        std::cout << "[DBNReader] invalid zstd frame" << std::endl;
        opened_ = false;
        return false;
    }

    if (decompressed_size == ZSTD_CONTENTSIZE_UNKNOWN)
    {
        std::cout << "[DBNReader] unknown zstd content size" << std::endl;
        opened_ = false;
        return false;
    }

    decompressed_data_.resize(static_cast<size_t>(decompressed_size));

    const size_t result = ZSTD_decompress(
        decompressed_data_.data(),
        decompressed_data_.size(),
        compressed.data(),
        compressed.size()
    );

    if (ZSTD_isError(result))
    {
        std::cout
            << "[DBNReader] zstd error: "
            << ZSTD_getErrorName(result)
            << std::endl;

        opened_ = false;
        return false;
    }

    std::cout
        << "[DBNReader] decompressed size: "
        << result
        << " bytes"
        << std::endl;

    opened_ = true;
    return true;
}

bool DBNReader::read_header()
{
    if (!opened_ || decompressed_data_.size() < sizeof(DBNHeader))
    {
        return false;
    }

    std::memcpy(
        &header_,
        decompressed_data_.data(),
        sizeof(DBNHeader)
    );

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
