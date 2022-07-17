#include "gtest/gtest.h"
#include "RollingChecksum.h"
#include <vector>


TEST(RollingCheckSumTests, ShouldInit){ // 12/2/2020 -> 737761

    std::vector<char>  test_data = {'h','o','w',' ','a','r','e',' ','y','o','u',' ','d','o','i','n','g'};
    RollingCheckSum rollingChecksum = {};
    rollingChecksum.init_from_array(test_data);

    auto checksum = rollingChecksum.check_sum();
    ASSERT_EQ(uint32_t (944178772), checksum);



}

TEST(RollingCheckSumTests,ShouldRollInAnOut){

    std::vector<char>  test_data = {'h','o','w',' ','a','r','e',' ','y','o','u',' ','d','o','i','n','g'};
    RollingCheckSum rollingChecksum = {};

    for (auto byte:test_data) {
        rollingChecksum.roll_in(byte);
    }

    auto checksum = rollingChecksum.check_sum();
    ASSERT_EQ(uint32_t (944178772), checksum);
    rollingChecksum.roll_out();
    ASSERT_EQ('h',rollingChecksum.get_removed());
    auto checksum2 = rollingChecksum.check_sum();
    ASSERT_NE(checksum,checksum2);
}


TEST(RollingCheckSumTests,ShouldReturnSameHashIfBuildingWithSameMethods){

    std::vector<char>  test_data = {'h','o','w',' ','a','r','e',' ','y','o','u',' ','d','o','i','n','g'};
    RollingCheckSum rollingChecksum = {};
    rollingChecksum.init_from_array(test_data);
    RollingCheckSum rollingChecksum2 = {};
    for (auto byte:test_data) {
        rollingChecksum2.roll_in(byte);
    }
    auto checksum = rollingChecksum.check_sum();
    auto checksum2 = rollingChecksum2.check_sum();
    ASSERT_EQ(checksum,checksum2);
}