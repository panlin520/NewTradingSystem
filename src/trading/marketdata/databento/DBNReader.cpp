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

    decompressed_data_.clear();

    ZSTD_DStream* stream = ZSTD_createDStream();

    if (!stream)
    {
        std::cout << "[DBNReader] create zstd stream failed" << std::endl;
        opened_ = false;
        return false;
    }

    size_t result = ZSTD_initDStream(stream);

    if (ZSTD_isError(result))
    {
        std::cout << "[DBNReader] init zstd stream failed: "
                  << ZSTD_getErrorName(result)
                  << std::endl;
        ZSTD_freeDStream(stream);
        opened_ = false;
        return false;
    }

    ZSTD_inBuffer input{
        compressed.data(),
        compressed.size(),
        0
    };

    std::vector<uint8_t> buffer(1024 * 1024);

    while (true)
    {
        ZSTD_outBuffer output{
            buffer.data(),
            buffer.size(),
            0
        };

        result = ZSTD_decompressStream(
            stream,
            &output,
            &input
        );

        if (ZSTD_isError(result))
        {
            std::cout << "[DBNReader] zstd stream error: "
                      << ZSTD_getErrorName(result)
                      << std::endl;
            ZSTD_freeDStream(stream);
            opened_ = false;
            return false;
        }

        decompressed_data_.insert(
            decompressed_data_.end(),
            buffer.begin(),
            buffer.begin() + output.pos
        );

        if (input.pos == input.size && result == 0)
        {
            break;
        }
    }

    ZSTD_freeDStream(stream);

    std::cout << "[DBNReader] decompressed size: "
              << decompressed_data_.size()
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
