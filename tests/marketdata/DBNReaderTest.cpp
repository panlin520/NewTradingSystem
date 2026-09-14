#include <gtest/gtest.h>

#include <array>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>

#include "trading/marketdata/databento/DBNReader.hpp"
#include "trading/marketdata/databento/DBNRecordDecoder.hpp"
#include "trading/marketdata/databento/MBOSchema.hpp"

using namespace CMETradingSystem::MarketData::Databento;

namespace
{

std::string TestDBNPath()
{
    return std::string(PROJECT_ROOT) +
           "/data/ESU6_2026-06-15_MBO.dbn.zst";
}

void write_u16_le(uint8_t* data, uint16_t value)
{
    data[0] = static_cast<uint8_t>(value & 0xFFU);
    data[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
}

void write_u32_le(uint8_t* data, uint32_t value)
{
    data[0] = static_cast<uint8_t>(value & 0xFFU);
    data[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
    data[2] = static_cast<uint8_t>((value >> 16U) & 0xFFU);
    data[3] = static_cast<uint8_t>((value >> 24U) & 0xFFU);
}

void write_u64_le(uint8_t* data, uint64_t value)
{
    data[0] = static_cast<uint8_t>(value & 0xFFULL);
    data[1] = static_cast<uint8_t>((value >> 8U) & 0xFFULL);
    data[2] = static_cast<uint8_t>((value >> 16U) & 0xFFULL);
    data[3] = static_cast<uint8_t>((value >> 24U) & 0xFFULL);
    data[4] = static_cast<uint8_t>((value >> 32U) & 0xFFULL);
    data[5] = static_cast<uint8_t>((value >> 40U) & 0xFFULL);
    data[6] = static_cast<uint8_t>((value >> 48U) & 0xFFULL);
    data[7] = static_cast<uint8_t>((value >> 56U) & 0xFFULL);
}

void write_i32_le(uint8_t* data, int32_t value)
{
    write_u32_le(data, std::bit_cast<uint32_t>(value));
}

void write_i64_le(uint8_t* data, int64_t value)
{
    write_u64_le(data, std::bit_cast<uint64_t>(value));
}

bool IsValidMBOAction(char action)
{
    return action == 'A' || action == 'C' || action == 'M' ||
           action == 'R' || action == 'T' || action == 'F' ||
           action == 'N';
}

bool IsValidMBOSide(char side)
{
    return side == 'A' || side == 'B' || side == 'N';
}

}

TEST(DBNReaderTest, OpenClose)
{
    std::cout << "[DBNReaderTest] START" << std::endl;

    const std::string path = TestDBNPath();

    std::cout << "[PATH] " << path << std::endl;

    const auto absolute_path = std::filesystem::absolute(path);

    std::cout << "[ABSOLUTE PATH] "
              << absolute_path.string()
              << std::endl;

    const bool exists = std::filesystem::exists(path);

    std::cout << "[EXISTS] "
              << (exists ? "YES" : "NO")
              << std::endl;

    ASSERT_TRUE(exists);

    std::cout << "[FILE SIZE] "
              << std::filesystem::file_size(path)
              << " bytes"
              << std::endl;

    DBNReader reader(path);

    EXPECT_TRUE(reader.open());
    EXPECT_TRUE(reader.is_open());

    std::cout << "[DBNReaderTest] OpenClose FINISHED" << std::endl;
}

TEST(DBNReaderTest, ReadHeader)
{
    std::cout << "[DBNReaderTest] ReadHeader START" << std::endl;

    DBNReader reader(TestDBNPath());

    ASSERT_TRUE(reader.open());

    std::cout << "[1/4] Reader opened" << std::endl;

    ASSERT_TRUE(reader.read_header());

    std::cout << "[2/4] Header read SUCCESS" << std::endl;

    const auto& header = reader.header();

    EXPECT_EQ(header.magic[0], 'D');
    EXPECT_EQ(header.magic[1], 'B');
    EXPECT_EQ(header.magic[2], 'N');

    // A valid DBN version comes from byte 3 of the prelude, not
    // byte 0 ('D' == 68), which was the previous parser bug.
    EXPECT_NE(header.version, static_cast<uint8_t>('D'));
    EXPECT_GT(header.version, 0U);

    EXPECT_GT(header.metadata_length, 0U);

    const size_t expected_records_offset =
        8U + static_cast<size_t>(header.metadata_length);

    EXPECT_EQ(reader.records_offset(), expected_records_offset);

    std::cout << "[3/4] DBN version: "
              << static_cast<unsigned int>(header.version)
              << std::endl;

    std::cout << "[4/4] Metadata length: "
              << header.metadata_length
              << " bytes, records offset: "
              << reader.records_offset()
              << std::endl;

    std::cout << "[DBNReaderTest] ReadHeader FINISHED" << std::endl;
}

TEST(DBNRecordDecoderTest, DecodeSyntheticMBO)
{
    std::array<uint8_t, MBOSchema::BASE_RECORD_SIZE> bytes{};

    constexpr uint16_t publisher_id = 1;
    constexpr uint32_t instrument_id = 123456;
    constexpr uint64_t ts_event = 1781481600123456789ULL;
    constexpr uint64_t order_id = 987654321012345ULL;
    constexpr int64_t price = 5975250000000LL;
    constexpr uint32_t size = 7;
    constexpr uint8_t flags = 0x20U;
    constexpr uint8_t channel_id = 3;
    constexpr char action = 'A';
    constexpr char side = 'B';
    constexpr uint64_t ts_recv = 1781481600123459999ULL;
    constexpr int32_t ts_in_delta = 3210;
    constexpr uint32_t sequence = 424242;

    bytes[MBOSchema::LENGTH_OFFSET] = MBOSchema::BASE_RECORD_LENGTH_WORDS;
    bytes[MBOSchema::RTYPE_OFFSET] = MBOSchema::MBO_RTYPE;
    write_u16_le(bytes.data() + MBOSchema::PUBLISHER_ID_OFFSET, publisher_id);
    write_u32_le(bytes.data() + MBOSchema::INSTRUMENT_ID_OFFSET, instrument_id);
    write_u64_le(bytes.data() + MBOSchema::TS_EVENT_OFFSET, ts_event);
    write_u64_le(bytes.data() + MBOSchema::ORDER_ID_OFFSET, order_id);
    write_i64_le(bytes.data() + MBOSchema::PRICE_OFFSET, price);
    write_u32_le(bytes.data() + MBOSchema::SIZE_OFFSET, size);
    bytes[MBOSchema::FLAGS_OFFSET] = flags;
    bytes[MBOSchema::CHANNEL_ID_OFFSET] = channel_id;
    bytes[MBOSchema::ACTION_OFFSET] = static_cast<uint8_t>(action);
    bytes[MBOSchema::SIDE_OFFSET] = static_cast<uint8_t>(side);
    write_u64_le(bytes.data() + MBOSchema::TS_RECV_OFFSET, ts_recv);
    write_i32_le(bytes.data() + MBOSchema::TS_IN_DELTA_OFFSET, ts_in_delta);
    write_u32_le(bytes.data() + MBOSchema::SEQUENCE_OFFSET, sequence);

    DBNRecordDecoder decoder;
    DBNRecord record{};

    ASSERT_TRUE(decoder.decode(bytes.data(), bytes.size(), record));

    EXPECT_EQ(record.rtype, MBOSchema::MBO_RTYPE);
    EXPECT_EQ(record.publisher_id, publisher_id);
    EXPECT_EQ(record.instrument_id, instrument_id);
    EXPECT_EQ(record.ts_event, ts_event);
    EXPECT_EQ(record.order_id, order_id);
    EXPECT_EQ(record.price, price);
    EXPECT_EQ(record.size, size);
    EXPECT_EQ(record.flags, flags);
    EXPECT_EQ(record.channel_id, channel_id);
    EXPECT_EQ(record.action, action);
    EXPECT_EQ(record.side, side);
    EXPECT_EQ(record.ts_recv, ts_recv);
    EXPECT_EQ(record.ts_in_delta, ts_in_delta);
    EXPECT_EQ(record.sequence, sequence);
    EXPECT_TRUE(record.symbol.empty());
}

TEST(DBNRecordDecoderTest, RejectTruncatedMBO)
{
    std::array<uint8_t, MBOSchema::BASE_RECORD_SIZE - 1> bytes{};

    bytes[MBOSchema::LENGTH_OFFSET] = MBOSchema::BASE_RECORD_LENGTH_WORDS;
    bytes[MBOSchema::RTYPE_OFFSET] = MBOSchema::MBO_RTYPE;

    DBNRecordDecoder decoder;
    DBNRecord record{};

    EXPECT_FALSE(decoder.decode(bytes.data(), bytes.size(), record));
}

TEST(DBNReaderTest, ReadFirstMBORecord)
{
    DBNReader reader(TestDBNPath());

    ASSERT_TRUE(reader.open());
    ASSERT_TRUE(reader.read_header());

    DBNRecord record{};
    bool found_mbo = false;

    constexpr size_t MAX_RECORDS_TO_SCAN = 10000;

    for (size_t i = 0; i < MAX_RECORDS_TO_SCAN; ++i)
    {
        ASSERT_TRUE(reader.next_record(record))
            << "Failed while scanning DBN record index " << i;

        if (record.rtype == MBOSchema::MBO_RTYPE)
        {
            found_mbo = true;
            break;
        }
    }

    ASSERT_TRUE(found_mbo)
        << "No MBO record found in the first "
        << MAX_RECORDS_TO_SCAN
        << " DBN records";

    EXPECT_GT(record.ts_event, 0ULL);
    EXPECT_GT(record.ts_recv, 0ULL);
    EXPECT_GT(record.publisher_id, 0U);
    EXPECT_GT(record.instrument_id, 0U);
    EXPECT_TRUE(IsValidMBOAction(record.action));
    EXPECT_TRUE(IsValidMBOSide(record.side));

    std::cout << "[FIRST MBO]"
              << " publisher_id=" << record.publisher_id
              << " instrument_id=" << record.instrument_id
              << " ts_event=" << record.ts_event
              << " ts_recv=" << record.ts_recv
              << " action=" << record.action
              << " side=" << record.side
              << " price=" << record.price
              << " size=" << record.size
              << " order_id=" << record.order_id
              << " sequence=" << record.sequence
              << std::endl;
}
