//
// Created by Juan Obligado on 15/07/2022.
//
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <queue>
#include <vector>
#include <queue>
#include "RollingChecksum.h"


//Constructor
RollingCheckSum::RollingCheckSum(){
    a = 0;
    b = 0;
    count = 0;
}

//Calculate checksum from byte array
RollingCheckSum& RollingCheckSum::init_from_array(std::vector<Byte>& data){
    a = 0;
    b = 0;
    count = 0;
    // Recursively update
    for(auto i =0 ; i < data.size();i++){
        auto byte = data[i];
        a += uint16_t(byte); // Create 16 bit
        b += uint16_t(data.size()-i)*uint16_t(byte);
        window.push_back(byte);
        count ++;
    }
    a %= M;
    b %= M;
    return *this;
}

uint32_t RollingCheckSum::check_sum(){
    //
    const int POW_16 = 16;
    const uint32_t MASK_16 = 0xFFFF; // 16 bit mask
    return  uint32_t(b) << POW_16 | uint32_t(a)&MASK_16;

}

//Push byte into checksum
RollingCheckSum& RollingCheckSum::roll_in(Byte input){
    a = (a + uint16_t(input)) % M;
    b = (a + b)%M;
    window.push_back(input);
    count ++;
    return *this;
}

//pulls item from checksum
RollingCheckSum& RollingCheckSum::roll_out(){
    // if nothing to rollout get back
    if(window.empty()){
        count = 0;
        return *this;
    }

    // Having at least one element to remove,
    last = window.front();
    auto removed_item_16 = uint16_t (last);
    a = ( a - removed_item_16) % M;
    b = ( b - (uint16_t (window.size())*removed_item_16 )) % M;
    // remove first element from list FIFO wise (could potentially replace with queue)
    window.erase(window.begin());
    count --;
    return *this;
}




