#include <gtest/gtest.h>

#include "trading/marketdata/databento/DBNReader.hpp"

using namespace CMETradingSystem::MarketData::Databento;


TEST(DBNReaderTest, OpenClose)
{
    DBNReader reader(
        "00_data/ESU6_2026-06-15_MBO.dbn.zst"
    );

    EXPECT_TRUE(reader.open());

    EXPECT_TRUE(reader.is_open());
}
