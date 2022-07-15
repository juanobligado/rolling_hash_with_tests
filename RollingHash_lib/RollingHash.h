#ifndef ROLLING_HASH_H_
#define ROLLING_HASH_H_


class RollingHash{

public:
    RollingHash(){};


    static int CalculateRollingHash(std::string& buffer, std::string& window);
};

#endif //ROLLING_HASH_H_