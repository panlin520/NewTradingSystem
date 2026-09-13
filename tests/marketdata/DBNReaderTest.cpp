#include <gtest/gtest.h>

#include <iostream>

#include "trading/marketdata/databento/DBNReader.hpp"

using namespace CMETradingSystem::MarketData::Databento;


TEST(DBNReaderTest, OpenClose)
{
    std::cout << "[DBNReaderTest] START" << std::endl;

    std::cout << "[1/4] Creating DBNReader" << std::endl;

    DBNReader reader(
        "00_data/ESU6_2026-06-15_MBO.dbn.zst"
    );

    std::cout << "[2/4] Opening DBN file" << std::endl;

    bool opened = reader.open();

    std::cout << "[3/4] Open result: "
              << (opened ? "SUCCESS" : "FAILED")
              << std::endl;

    EXPECT_TRUE(opened);

    std::cout << "[4/4] Checking reader state" << std::endl;

    EXPECT_TRUE(reader.is_open());

    std::cout << "[DBNReaderTest] FINISHED" << std::endl;
}
