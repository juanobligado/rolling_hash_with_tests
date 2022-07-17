#include <vector>
#include "RsyncFileDiff.h"
#include "Sha1.hpp"

//Returns List with weak and strong hash values for each block
std::vector<HashItem> RsyncFileDiff::calculate_hash_table(std::istream&  input){
    //read chunks from file
    std::vector<HashItem> result = {};
    std::vector<Byte> buffer(block_size);
    auto reading = true;
    while (!input.eof()){
        input.read(buffer.data(),buffer.size());
        auto currentChunkSize = input.gcount();
        if(currentChunkSize == 0)
            break;

        if(currentChunkSize < block_size )
            buffer.resize(currentChunkSize);

        auto hashItem =  create_block_hash_item(buffer);
        result.push_back(hashItem);


    }
    return result;

}

// Calculates simple 32-bit rolling checksum
uint32_t RsyncFileDiff::simple(std::vector<Byte> &chunk) {
    return RollingCheckSum().init_from_array(chunk).check_sum();
}
// Calculates SHA1 Hash from bytes
std::string  RsyncFileDiff::strong(std::vector<Byte> &buffer) {
    SHA1 sha1 = {};
    sha1.update(buffer);
    auto str= sha1.final();
    return str;
}

HashItem RsyncFileDiff::create_block_hash_item(std::vector<Byte> &chunk)
{
    auto hi = HashItem();
    hi.simple_hash = simple(chunk);
    hi.full_hash = strong(chunk);
    return hi;
}


std::shared_ptr<SimpleHashToComplexMap> RsyncFileDiff::build_indexes(std::vector<HashItem> &block_hashes){
    //Todo Build Indexes from blocks
    auto indexes = std::make_shared<SimpleHashToComplexMap>();
    for(int  iBlockNo=0; iBlockNo < block_hashes.size(); iBlockNo++){
        auto blockHash= block_hashes[iBlockNo];
        // assign strong Hash to chunk index map
        auto hash_to_index = std::map<std::string ,int>({{blockHash.full_hash,iBlockNo}});
        indexes->insert_or_assign(blockHash.simple_hash,hash_to_index);
    }
return indexes;
}

// Creates block diff summary
BlockDiffResult RsyncFileDiff::create_diff_block(int index, std::vector<Byte> &mismatchedChars){
    auto diff =  BlockDiffResult();
    diff.start = index*block_size;
    diff.offset = diff.start + block_size;
    diff.literals = std::vector<Byte>(mismatchedChars);
    return  diff;
}

//Looks for block comparison on src hash map and returns blockNumber of it if any
// Returns src file matching block-number
int RsyncFileDiff::find_matching_block(std::shared_ptr<SimpleHashToComplexMap> block_hash_map, RollingCheckSum& checkSum )
{
    //First check if having light checksum on src block hash map
    auto simpleHash = checkSum.check_sum();
    auto checksum_map = block_hash_map->find(simpleHash);

    if(checksum_map == block_hash_map->end())
        return -1;


    // calculate sha-1 for checksum window to see if having hard match as well
    auto dst_sha_hash = strong(checkSum.get_window());

    // Although 90% time chechsum_map should only contain 1 item might have multiple blocks with same checksum
    auto full_hash_it = checksum_map->second.find(dst_sha_hash);

    //
    if(full_hash_it!= checksum_map->second.end())
        return full_hash_it->second;

    // nothing found
    return -1;

}

// Calculate differences between hash map and other stream
std::shared_ptr<Differences> RsyncFileDiff::calculate_differences(std::istream& src , std::istream &other) {

    auto result = std::make_shared<Differences>();
    auto block_hashes = calculate_hash_table(src);

    //create block_hash to index map
    auto block_hash_map = build_indexes(block_hashes);
    std::vector<char> tmpDiffs = {};
    auto rollingChecksum = RollingCheckSum();


    //Iterate
    if(other.bad())
        return  result;

    // read new stream byte by byte
    while (!other.eof()){
        std::vector<Byte> buffer(1);
        other.read(buffer.data(),1);
        if(other.gcount()>0){
            rollingChecksum.roll_in(buffer[0]);
            //keep reading until buffer is initialized with data
            if(rollingChecksum.get_count() < block_size)
                continue;

            // rolling checksum > blocksize : On over read continue pushing diffed chars
            if(rollingChecksum.get_count() > block_size){
                rollingChecksum.roll_out();
                auto last_removed = rollingChecksum.get_removed();
                tmpDiffs.push_back(last_removed);
            }

            // calc simple checksum not that rolling checksum size == chunk_size
            auto matchingBlockNumber = find_matching_block(block_hash_map,rollingChecksum);
            if(matchingBlockNumber != -1){

                auto newBlock = create_diff_block(matchingBlockNumber,tmpDiffs);
                result->insert_or_assign(matchingBlockNumber,newBlock);
                // Clean quick checksum and diff []
                rollingChecksum.clear();
                tmpDiffs.clear();
            }

        };// end for

    }
    //Process last block
    if(rollingChecksum.get_count() > 0 && rollingChecksum.get_count() < block_size){
        // calc simple checksum not that rolling checksum size == chunk_size
        auto matchingBlockNumber = find_matching_block(block_hash_map,rollingChecksum);
        if(matchingBlockNumber != -1){

            auto newBlock = create_diff_block(matchingBlockNumber,tmpDiffs);
            result->insert_or_assign(matchingBlockNumber,newBlock);
            // Clean quick checksum and diff []
            rollingChecksum.clear();
            tmpDiffs.clear();
        }
    }
    rollingChecksum.clear();
    tmpDiffs.clear();

    //add missing blocks into result ( index is in sig but not in result map)
    for(int iBlock=0;iBlock<block_hashes.size();iBlock++){
        //Include block as missing if not in result
        auto matching_it = result->find(iBlock);
        if(matching_it == result->end()){
            // Missing block add missing summary
            auto buff = std::vector<Byte>();
            auto missingBlock = create_diff_block(iBlock,buff);
            missingBlock.missing = true;
            result->insert_or_assign(iBlock,missingBlock);
        }
    }
    return result;
}



