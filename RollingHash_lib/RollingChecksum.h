//
// Created by Juan Obligado on 16/07/2022.
//
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <queue>
#include <vector>
#include <queue>
#include "CustomTypeDefinitions.h"
#ifndef TESTING_ROLLING_HASH_ROLLINGCHECKSUM_H
#define TESTING_ROLLING_HASH_ROLLINGCHECKSUM_H
class RollingCheckSum{
    private:

        std::vector<Byte> window;
        int count;
        uint8_t last; // last rolled item
        uint16_t a ; // a(k,l) = \sum{i=k,l}(X_i) Mod M
        uint16_t b;  // b(k,l) = \sum{i=k,l}( (l -i +1)*X_i ) Mod M
        const int  M = 65521;
    public:
        RollingCheckSum();

        //Initialize from vector
        RollingCheckSum& init_from_array(std::vector<Byte>& data);
        uint32_t check_sum();
        //Push byte into checksum
        RollingCheckSum& roll_in(Byte input);
        //pulls item from checksum
        RollingCheckSum& roll_out();

        uint16_t get_a(){return a;}
        uint16_t get_b(){return b;}
        int get_count(){return count;}
        uint8_t get_removed(){return  last;}
        std::vector<Byte>& get_window(){return  window;}

};
#endif //TESTING_ROLLING_HASH_ROLLINGCHECKSUM_H
