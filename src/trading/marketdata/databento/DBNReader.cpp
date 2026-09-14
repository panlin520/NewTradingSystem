#include "trading/marketdata/databento/DBNReader.hpp"
#include "trading/marketdata/databento/DBNRecordDecoder.hpp"
#include "trading/marketdata/databento/DBNRecordHeader.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>

#include <zstd.h>

namespace CMETradingSystem::MarketData::Databento
{

namespace
{

constexpr size_t DBN_RECORD_LENGTH_MULTIPLIER = 4;

uint32_t read_u32_le(const uint8_t* data) noexcept
{
    return static_cast<uint32_t>(data[0]) |
           (static_cast<uint32_t>(data[1]) << 8U) |
           (static_cast<uint32_t>(data[2]) << 16U) |
           (static_cast<uint32_t>(data[3]) << 24U);
}

}

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

    const std::streampos end_position = file.tellg();

    if (end_position <= 0)
    {
        opened_ = false;
        return false;
    }

    const auto compressed_size = static_cast<size_t>(end_position);
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> compressed(compressed_size);

    if (!file.read(
            reinterpret_cast<char*>(compressed.data()),
            static_cast<std::streamsize>(compressed.size())))
    {
        opened_ = false;
        return false;
    }

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

        result = ZSTD_decompressStream(stream, &output, &input);

        if (ZSTD_isError(result))
        {
            ZSTD_freeDStream(stream);
            opened_ = false;
            return false;
        }

        decompressed_data_.insert(
            decompressed_data_.end(),
            buffer.begin(),
            buffer.begin() + static_cast<std::ptrdiff_t>(output.pos)
        );

        if (input.pos == input.size && result == 0)
        {
            break;
        }

        // No input was consumed and no output was produced. Continuing in
        // this state would spin forever on malformed/truncated input.
        if (output.pos == 0 && input.pos == input.size && result != 0)
        {
            ZSTD_freeDStream(stream);
            opened_ = false;
            return false;
        }
    }

    ZSTD_freeDStream(stream);

    header_ = {};
    current_offset_ = 0;
    records_offset_ = 0;
    opened_ = true;

    std::cout << "[DBNReader] decompressed size: "
              << decompressed_data_.size()
              << " bytes"
              << std::endl;

    return true;
}

bool DBNReader::read_header()
{
    if (!opened_ || decompressed_data_.size() < DBN_PRELUDE_SIZE)
    {
        return false;
    }

    const uint8_t* data = decompressed_data_.data();

    if (data[0] != static_cast<uint8_t>('D') ||
        data[1] != static_cast<uint8_t>('B') ||
        data[2] != static_cast<uint8_t>('N'))
    {
        return false;
    }

    DBNHeader parsed{};
    parsed.magic[0] = 'D';
    parsed.magic[1] = 'B';
    parsed.magic[2] = 'N';
    parsed.version = data[3];
    parsed.metadata_length = read_u32_le(data + 4);

    const size_t metadata_length = static_cast<size_t>(parsed.metadata_length);

    if (metadata_length >
        std::numeric_limits<size_t>::max() - DBN_PRELUDE_SIZE)
    {
        return false;
    }

    const size_t first_record_offset = DBN_PRELUDE_SIZE + metadata_length;

    if (first_record_offset > decompressed_data_.size())
    {
        return false;
    }

    header_ = parsed;
    records_offset_ = first_record_offset;
    current_offset_ = first_record_offset;

    return true;
}

bool DBNReader::next_record(DBNRecord& record)
{
    if (!opened_ || records_offset_ == 0 ||
        current_offset_ >= decompressed_data_.size())
    {
        return false;
    }

    const uint8_t* data = decompressed_data_.data() + current_offset_;
    const size_t remaining = decompressed_data_.size() - current_offset_;

    if (remaining < sizeof(DBNRecordHeader))
    {
        return false;
    }

    DBNRecordHeader record_header{};
    std::memcpy(&record_header, data, sizeof(DBNRecordHeader));

    if (record_header.length == 0)
    {
        return false;
    }

    const size_t record_size =
        static_cast<size_t>(record_header.length) *
        DBN_RECORD_LENGTH_MULTIPLIER;

    if (record_size < sizeof(DBNRecordHeader) || record_size > remaining)
    {
        return false;
    }

    DBNRecordDecoder decoder;

    if (!decoder.decode(data, record_size, record))
    {
        return false;
    }

    current_offset_ += record_size;

    return true;
}

bool DBNReader::is_open() const noexcept
{
    return opened_;
}

bool DBNReader::at_end() const noexcept
{
    return opened_ &&
           records_offset_ != 0 &&
           current_offset_ >= decompressed_data_.size();
}

const DBNHeader& DBNReader::header() const noexcept
{
    return header_;
}

size_t DBNReader::records_offset() const noexcept
{
    return records_offset_;
}

}
