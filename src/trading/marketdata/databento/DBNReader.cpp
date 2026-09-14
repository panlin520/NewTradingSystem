#include "trading/marketdata/databento/DBNReader.hpp"
#include "trading/marketdata/databento/DBNRecordDecoder.hpp"

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

    decompressed_data_.clear();

    ZSTD_DStream* stream = ZSTD_createDStream();

    if (!stream)
    {
        opened_ = false;
        return false;
    }

    size_t result = ZSTD_initDStream(stream);

    if (ZSTD_isError(result))
    {
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

    current_offset_ = 0;
    opened_ = true;

    std::cout << "[DBNReader] decompressed size: "
              << decompressed_data_.size()
              << " bytes"
              << std::endl;

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

    current_offset_ = sizeof(DBNHeader);

    return true;
}

bool DBNReader::next_record(DBNRecord& record)
{
    if (!opened_)
    {
        return false;
    }

    if (current_offset_ >= decompressed_data_.size())
    {
        return false;
    }

    DBNRecordDecoder decoder;

    const uint8_t* data = decompressed_data_.data() + current_offset_;
    const size_t remaining = decompressed_data_.size() - current_offset_;

    if (!decoder.decode(data, remaining, record))
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
