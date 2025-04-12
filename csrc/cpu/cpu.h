#ifndef CPU_H
#define CPU_H
#include "../include/utils.h"
#include "verilated_fst_c.h"
#include "cache.h"

template <size_t NumRegisters = 32, size_t NumCacheSets = 128, size_t CacheBlockSize = 4>
struct CPU {
    // Registers
    std::map<std::string, int> reg;

    // Cache
    Cache<NumCacheSets, CacheBlockSize> cache;

    CPU() {
        reg.clear();
    }

    // Read from memory (via cache)
    bool read_memory(uint32_t address, uint32_t& data) {
        return cache.access(address, data);
    }

    // Write to memory (via cache)
    void write_memory(uint32_t address, const uint32_t* data_block) {
        cache.update(address, data_block);
    }
};

#endif