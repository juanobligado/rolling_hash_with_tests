#include "gtest/gtest.h"
#include "RollingHash.h"
#include <iostream>
#include <fstream>
#include <filesystem>



TEST(RollingHashTestSuite, ShouldCreateHashTable){ // 12/2/2020 -> 737761
    auto rolling_hash = RollingHash(8);// 8 byte hash
    auto current = std::filesystem::current_path();
    std::cout<<current;
    auto have_file = std::filesystem::exists("./Input.txt/Input.txt");
    std::ifstream test_file("./Input.txt/Input.txt");

    ASSERT_TRUE(test_file.good());
    auto table = rolling_hash.calculate_hash_table(test_file);
    EXPECT_EQ(9,table.size());
}


TEST(RollingHashTestSuite, IncorrectDate){ // 12/0/2020 -> 0
    // GregorianDate gregDate;
    // gregDate.SetMonth(12);
    // gregDate.SetDay(0);
    // gregDate.SetYear(2020);

    ASSERT_EQ(1,0);
}