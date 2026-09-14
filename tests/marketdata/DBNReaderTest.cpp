#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "trading/marketdata/databento/DBNReader.hpp"

using namespace CMETradingSystem::MarketData::Databento;


TEST(DBNReaderTest, OpenClose)
{
    std::cout << "[DBNReaderTest] START" << std::endl;

    std::cout << "[1/5] Creating DBNReader" << std::endl;

    const std::string path =
        "data/ESU6_2026-06-15_MBO.dbn.zst";

    std::cout << "[PATH] "
              << path
              << std::endl;

    const auto absolute_path =
        std::filesystem::absolute(path);

    std::cout << "[ABSOLUTE PATH] "
              << absolute_path.string()
              << std::endl;

    bool exists =
        std::filesystem::exists(path);

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

    std::cout << "[2/5] Opening DBN file" << std::endl;

    bool opened = reader.open();

    std::cout << "[3/5] Open result: "
              << (opened ? "SUCCESS" : "FAILED")
              << std::endl;

    EXPECT_TRUE(opened);

    std::cout << "[4/5] Checking reader state" << std::endl;

    EXPECT_TRUE(reader.is_open());

    std::cout << "[5/5] DBNReader validation finished" << std::endl;

    std::cout << "[DBNReaderTest] FINISHED" << std::endl;
}
