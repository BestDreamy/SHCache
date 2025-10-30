#ifndef CACHE_H
#define CACHE_H
#include "../include/utils.h"
#include "../include/dbg.h"
#include "../mem.h"
#include "../include/autoconfig.h"
#include <cstddef>
#include <cstring>

enum Cache_State {
    I = 0,
    SC = 1,
    UC = 2,
    UD = 6,
    SD = 7,

    Save = 3 // save the origin state
};

// Direct-Map Local Cache
template <size_t numSet = 128, size_t BlockSize = 4>
struct Cache {
    static constexpr size_t numBlock = 1 << BlockSize;
    paddr_t tag_array[numSet];
    uint8_t data_array[numSet][numBlock]; // 4 int per set
    Cache_State val_array[numSet];

    Cache() {
        for (size_t i = 0; i < numSet; ++i) {
            tag_array[i] = 0;
            val_array[i] = I;
            for (size_t j = 0; j < numBlock; ++j) {
                data_array[i][j] = 0;
            }
        }
    }

    inline paddr_t offset_of(paddr_t addr) const {
        return addr % numBlock;
    }

    inline paddr_t aligned_of(paddr_t addr) const {
        return addr & ~(numBlock - 1);
    }

    inline paddr_t set_of(paddr_t addr) const {
        paddr_t aligned_addr = aligned_of(addr);
        paddr_t index = (addr / numBlock) % numSet; // Extract index from address
        return index;
    }

    inline paddr_t tag_of(paddr_t addr) const {
        paddr_t aligned_addr = aligned_of(addr);
        paddr_t tag = addr / (numSet * numBlock); // Extract index from address
        return tag;
    }

    /*
    __________________________________________________________________
    | Tag (21 bits)       | Index (7 bits)   | Block Offset (4 bits) |
    ------------------------------------------------------------------
    */

    bool is_hit(const paddr_t &addr) const {        
        return (val_array[set_of(addr)] and tag_array[set_of(addr)] == tag_of(addr));
    }

    bool is_unique(const paddr_t &addr) const {
        return ((val_array[set_of(addr)] == UC or val_array[set_of(addr)] == UD) and tag_array[set_of(addr)] == tag_of(addr));
    }

    void update_cacheline_by_byte(const paddr_t &addr, const uint8_t& data, const Cache_State &state) {
        paddr_t offset = offset_of(addr);
        paddr_t index = set_of(addr);
        paddr_t tag = tag_of(addr);
        
        // Update the cache line
        tag_array[index] = tag;
        if (state != Save) val_array[index] = state;
        memcpy(&data_array[index][offset], &data, sizeof(data));
    }

    void update_cacheline_by_word(const paddr_t &addr, const uint32_t& data, const Cache_State &state) {
        paddr_t offset = offset_of(addr);
        paddr_t index = set_of(addr);
        paddr_t tag = tag_of(addr);
        
        // Update the cache line
        tag_array[index] = tag;
        if (state != Save) val_array[index] = state;
        for (size_t i = 0; i < 4; ++i) {
            memcpy(&data_array[index][offset + i], ((uint8_t*)&data) + i, sizeof(uint8_t));
        }
    }

    bool access(
        const int &coreId, const paddr_t &addr, uint32_t& data
    );
    
    bool update(
        const int &coreId, const paddr_t &addr, const uint32_t& new_data
    );

    void show_cache() const;
};

#endif
