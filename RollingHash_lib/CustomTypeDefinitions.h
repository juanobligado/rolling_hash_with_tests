//
// Created by Juan Obligado on 16/07/2022.
//
#include <string>
#include <map>

#ifndef TESTING_ROLLING_HASH_CUSTOMTYPEDEFINITIONS_H
#define TESTING_ROLLING_HASH_CUSTOMTYPEDEFINITIONS_H
typedef char Byte;
typedef std::map<uint32_t, std::map<std::string, int >> SimpleHashToComplexMap;

/**
 * Block Difference Result
 * Missing = true && no literal BlockMissing
 * Missing = false && len(lit) > 0 Some changes
 * Missing = false && len(lit) == 0 100% block Match
 *
 */
struct BlockDiffResult {
    int offset;
    int start;
    bool missing;
    std::vector<char> literals;
};

//Maps block index to difference
typedef std::map<int, BlockDiffResult> Differences;

// contains simple and full hash for rsync algo implementation
struct HashItem {
    // simple checksum hash
    std::uint32_t simple_hash;
    //full block hash (sha-1)
    std::string full_hash;
};
#endif //TESTING_ROLLING_HASH_CUSTOMTYPEDEFINITIONS_H
