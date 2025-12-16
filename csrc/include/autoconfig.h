#ifndef CONFIG_H
#define CONFIG_H
#include <cstdint>

#define NUMCORES 2

struct Config {
    static constexpr uint32_t numRNs = NUMCORES;
    static constexpr uint32_t numHNs = 1;
    static constexpr uint32_t numSNs = 1;
    static constexpr uint32_t numCreditsForHNReq[1] = {4};
    static constexpr uint32_t numCreditsForSNReq[1] = {4};
    static constexpr uint32_t numCreditsForHNRsp[1] = {4};
    static constexpr uint32_t RNId[NUMCORES] = {0, 1};
    static constexpr uint32_t HNId[1] = {5};
    static constexpr uint32_t SNId[1] = {6};
    static constexpr uint32_t RNsOffset[1] = {4};
};

extern Config config;
#endif