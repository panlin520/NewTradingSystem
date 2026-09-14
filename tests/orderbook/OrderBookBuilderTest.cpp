#include <gtest/gtest.h>

#include "trading/orderbook/OrderBookBuilder.hpp"

using namespace CMETradingSystem::Trading::OrderBook;

TEST(OrderBookBuilderTest, ProcessMBOActions)
{
    OrderBookBuilder builder;

    EXPECT_TRUE(builder.process('A', 'B', 1, 5000, 10, 1, 1));
    EXPECT_EQ(builder.book().order_count(), 1);
    EXPECT_EQ(builder.book().bid_volume(), 10);

    EXPECT_TRUE(builder.process('M', 'B', 1, 5000, 20, 2, 2));
    EXPECT_EQ(builder.book().bid_volume(), 20);

    EXPECT_TRUE(builder.process('C', 'B', 1, 5000, 0, 3, 3));
    EXPECT_EQ(builder.book().order_count(), 0);

    EXPECT_TRUE(builder.process('R', 'N', 0, 0, 0, 4, 4));
    EXPECT_EQ(builder.book().order_count(), 0);
}
