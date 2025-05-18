
#ifndef CONFIG_H
#define CONFIG_H
#include <vector>
#include <cstdint>

struct Config {
    static constexpr uint32_t numRNs = 4;
    static constexpr uint32_t numHNs = 1;
    static constexpr uint32_t numSNs = 1;

    static constexpr uint32_t numCreditsForHNReq[1] = {4};
    static constexpr uint32_t numCreditsForSNReq[1] = {4};

    static constexpr uint32_t RNId[4] = {0x1, 0x2, 0x3, 0x4};
    static constexpr uint32_t HNId[1] = {0x5};
    static constexpr uint32_t SNId[1] = {0x6};

    static constexpr uint32_t RNsOffset[1] = {4};
};

extern Config config;

#endif
