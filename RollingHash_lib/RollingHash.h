#ifndef ROLLING_HASH_H_
#define ROLLING_HASH_H_
#include <string>
#include <map>
#include <istream>
#include "RollingChecksum.h"
#include "CustomTypeDefinitions.h"


/**
 * Block Difference
 * Missing = true && no literal BlockMissing
 * Missing = false && len(lit) > 0 Some changes
 * Missing = false && len(lit) == 0 100% block Match
 *
 */
struct Bytes{
    int offset;
    int start;
    bool missing;
    std::vector<char> literals;
};



typedef std::map<int,Bytes> Differences;

 struct HashItem{
     // simple checksum hash
     std::uint32_t simple_hash;
     std::string full_hash;
 };
#define S = 16;
class RollingHash{
private:
     int block_size = 0;
     Bytes create_diff_block(int index,std::vector<char>& lits){
        return Bytes{
                index*block_size + block_size,
                index*block_size,
                false,
                lits
        };
    }

public:
    RollingHash(int blockSize){
        block_size = blockSize;

    };

    std::vector<HashItem> calculate_hash_table(std::istream& buffer);


    Differences calculate_differences(std::vector<HashItem>& sig, std::istream& other);

    static int CalculateRollingHash(std::string& buffer, std::string& window);




    std::shared_ptr<Indexes> build_indexes(std::vector<HashItem>& signatures){
        //Todo Build Indexes from blocks
        auto indexes = std::make_shared<Indexes>();
        for(auto  i=0;i<signatures.size();i++){
            auto sig= signatures[i];
            // assign strong Hash to chunk index map
            auto hash_to_index = std::map<std::string ,int>({{sig.full_hash,i}});
            indexes->insert_or_assign(sig.simple_hash,hash_to_index);

        }
        return indexes;
    }

    int find_matching_block(std::shared_ptr<Indexes> idx,uint32_t checksum,std::vector<Byte>& buffer){
        //check if we do have weak hash in indexTable
        auto checksum_map = idx->find(checksum);
        if(checksum_map == idx->end())
            return -1;

        auto matchingHash = strong(buffer);
        auto full_hash_it = checksum_map->second.find(matchingHash);
        if(full_hash_it!= checksum_map->second.end())
            return full_hash_it->second;
        return -1;

    }

    //Calculates strong hash (SHA-1)
    static std::string strong(std::vector<Byte>& buffer);


    static HashItem CreateHashItem(std::vector<Byte>& chunk){
        auto hi = HashItem();
        auto rcs = RollingCheckSum().init_from_array(chunk);
        hi.simple_hash = rcs.check_sum();
        hi.full_hash = strong(chunk);
        return hi;
    }


};

#endif //ROLLING_HASH_H_