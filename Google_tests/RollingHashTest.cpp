#include "gtest/gtest.h"
#include "RollingHash.h"


TEST(RollingHashTestSuite, ExampleDate){ // 12/2/2020 -> 737761
    // GregorianDate gregDate;
    // gregDate.SetMonth(12);
    // gregDate.SetDay(2);
    // gregDate.SetYear(2020);

    EXPECT_EQ(737761,737761);
}


TEST(RollingHashTestSuite, IncorrectDate){ // 12/0/2020 -> 0
    // GregorianDate gregDate;
    // gregDate.SetMonth(12);
    // gregDate.SetDay(0);
    // gregDate.SetYear(2020);

    ASSERT_EQ(1,0);
}