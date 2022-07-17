//
// Created by Juan Obligado on 16/07/2022.
//
#include "gtest/gtest.h"
#include "sha1.hpp"
#include <vector>


TEST(Sha1Tests, ShouldInit){ 

    std::vector<char>  test_data = {'h','o','w',' ','a','r','e',' ','y','o','u',' ','d','o','i','n','g'};
    auto sha1 = SHA1();
    sha1.update(test_data);

    auto hash1 = sha1.final();

    auto sha1_bis = SHA1();
    sha1_bis.update("how are you doing");


    auto hash2 = sha1_bis.final();
    ASSERT_STREQ(hash1.c_str(), hash2.c_str());



}

