//
// Created by Juan Obligado on 16/07/2022.
//
#ifndef ROLLING_HASH_H_
#define ROLLING_HASH_H_


#include <string>
#include <map>
#include <istream>
#include "RollingChecksum.h"
#include "CustomTypeDefinitions.h"

#define S = 16;
class RsyncFileDiff{




    public:
        RsyncFileDiff(int blockSize){
            block_size = blockSize;

        };

        std::vector<HashItem> calculate_hash_table(std::istream& buffer);

        // Process differences
        std::shared_ptr<Differences> calculate_differences(std::istream& original, std::istream& other);



    private:
        int block_size = 0;
        BlockDiffResult create_diff_block(int index,std::vector<Byte>& mismatchedChars);


        std::shared_ptr<SimpleHashToComplexMap> build_indexes(std::vector<HashItem>& signatures);

        int find_matching_block(std::shared_ptr<SimpleHashToComplexMap> idx,RollingCheckSum&  checksum);

        //Calculates strong hash (SHA-1)
        static std::string strong(std::vector<Byte>& buffer);
        static uint32_t simple(std::vector<Byte>& buffer);
        static HashItem create_block_hash_item(std::vector<Byte>& chunk);

};

#endif //ROLLING_HASH_H_