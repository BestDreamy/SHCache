
#ifndef CONFIG_H
#define CONFIG_H
#include <vector>
#include <cstdint>

struct Config {
    uint32_t numRNs = 4;
    uint32_t numHNs = 1;
    uint32_t numSNs = 1;

    std::vector<uint32_t> numCreditsForHNReq = {4};
    std::vector<uint32_t> numCreditsForSNReq = {4};

    std::vector<uint32_t> RNId = {0x0, 0x1, 0x2, 0x3};
    std::vector<uint32_t> HNId = {0x28};
    std::vector<uint32_t> SNId = {0x24};
};

extern Config config;

#endif
