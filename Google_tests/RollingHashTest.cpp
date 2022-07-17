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

    //close file
    test_file.close();
}

TEST(RollingHashTestSuite, SameFileShouldReturnNoDiff){ // 12/2/2020 -> 737761
    auto rolling_hash = RollingHash(8);// 8 byte hash
    auto current = std::filesystem::current_path();
    std::cout<<current;
    auto have_file = std::filesystem::exists("./Input.txt/Input.txt");
    std::ifstream test_file("./test_inputs/Input.txt");
    std::ifstream test_file_2("./test_inputs/Input.txt");

    auto diffs = rolling_hash.calculate_differences(test_file,test_file_2);

    ASSERT_EQ(9, diffs->size());
    for(auto diff : *diffs){
        ASSERT_EQ(false,diff.second.missing);
    }
    //close file
    test_file.close();
    test_file_2.close();
}

TEST(RollingHashTestSuite, DifferentFileShouldReturnDiff){ // 12/2/2020 -> 737761
    auto rolling_hash = RollingHash(8);// 8 byte hash
    auto current = std::filesystem::current_path();
    std::cout<<current;
    auto have_file = std::filesystem::exists("./Input.txt/Input.txt");
    std::ifstream test_file("./test_inputs/Input.txt");
    std::ifstream test_file_2("./test_inputs/ModifiedInput.txt");

    auto diffs = rolling_hash.calculate_differences(test_file,test_file_2);

    ASSERT_EQ(9, diffs->size());

    //close file
    test_file.close();
    test_file_2.close();
}


