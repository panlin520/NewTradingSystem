#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "trading/marketdata/databento/DBNReader.hpp"

using namespace CMETradingSystem::MarketData::Databento;


static std::string TestDBNPath()
{
    return "../../../../data/ESU6_2026-06-15_MBO.dbn.zst";
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

    bool exists = std::filesystem::exists(path);

    std::cout << "[EXISTS] "
              << (exists ? "YES" : "NO")
              << std::endl;

    EXPECT_TRUE(exists);

    if (exists)
    {
        std::cout << "[FILE SIZE] "
                  << std::filesystem::file_size(path)
                  << " bytes"
                  << std::endl;
    }

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

    std::cout << "[1/3] Reader opened" << std::endl;

    bool result = reader.read_header();

    std::cout << "[2/3] Header read result: "
              << (result ? "SUCCESS" : "FAILED")
              << std::endl;

    EXPECT_TRUE(result);

    const auto& header = reader.header();

    std::cout << "[3/3] Header version: "
              << static_cast<int>(header.version)
              << std::endl;

    std::cout << "[DBNReaderTest] ReadHeader FINISHED" << std::endl;
}
