#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "trading/marketdata/databento/DBNReader.hpp"

using namespace CMETradingSystem::MarketData::Databento;


static std::string TestDBNPath()
{
    return std::string(PROJECT_ROOT) +
           "/data/ESU6_2026-06-15_MBO.dbn.zst";
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

    // A valid DBN version must come from byte 3 of the prelude, not
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
