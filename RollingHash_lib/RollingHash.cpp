#include <vector>
#include "RollingHash.h"
#include "sha1.hpp"
//Rolling Hash
int RollingHash::CalculateRollingHash(std::string &buffer, std::string &window) {
    //TODO: Implement Rolling Hash algo
    return 0;
}

std::vector<HashItem> RollingHash::calculate_hash_table(std::istream&  input){
    //read chunks from file
    std::vector<HashItem> result = {};
    std::vector<Byte> buffer(block_size);
    auto reading = true;
    while (!input.eof()){
        input.read((char *)buffer.data(),buffer.size());
        std::streamsize currentChunkSize = input.gcount();
        auto hashItem = RollingHash::CreateHashItem(buffer);
        hashItem.full_hash = strong(buffer);
        result.push_back(hashItem);
     }
    return result;

}

 std::string  RollingHash::strong(std::vector<Byte> &buffer) {

    auto sha1 = SHA1();
    sha1.update(buffer);
    auto str= sha1.final();
    return str;
}
/**
 * Calculates differences between hash map and other stream
 * @param sig
 * @param other
 * @return
 */
Differences RollingHash::calculate_differences(std::vector<HashItem> &sig, std::istream &other) {


    Differences result = {};
    auto rollingChecksum = RollingCheckSum();
    auto indexes = build_indexes(sig);
    std::vector<char> tmpDiffs = {};

    //Iterate
    if(other.bad())
        return  result;

    while (!other.eof()){
        char* bytePtr = NULL;
        other.read(bytePtr,1);
        if(other.gcount()>0){
            rollingChecksum.roll_in(*bytePtr);
            //keep reading until buffer is initialized with data
            if(rollingChecksum.get_count() < block_size)
                continue;

            // Buffer is full start rolling data
            if(rollingChecksum.get_count() > block_size){
                rollingChecksum.roll_out();
                auto last_removed = rollingChecksum.get_removed();
                tmpDiffs.push_back(last_removed);
            }

            // calc simple checksum not that rolling checksum size == chunk_size
            auto matchingIndex = find_matching_block(indexes,rollingChecksum.check_sum(),rollingChecksum.get_window());
            if(matchingIndex != -1){
                auto newBlock = create_diff_block(matchingIndex,tmpDiffs);
                result[matchingIndex] = newBlock;
            }

        };
    }

    return result;
}