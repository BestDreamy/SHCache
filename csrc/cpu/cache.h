#ifndef CACHE_H
#define CACHE_H
#include "../include/utils.h"
#include "../mem.h"
#include "../chi/transaction.h"

enum cpuCacheState {
    I = 0,
    SC = 1,
    UC = 2,
    UD = 6,
    SD = 7
};

// Direct-Map Local Cache
template <size_t NumSets = 128, size_t BlockSize = 4>
struct Cache {
    uint32_t tag_array[NumSets];
    uint32_t data_array[NumSets][BlockSize];
    cpuCacheState val_array[NumSets];
    bool access_finished;

    Cache() {
        access_finished = false;
        
        for (size_t i = 0; i < NumSets; ++i) {
            tag_array[i] = 0;
            val_array[i] = 0;
            for (size_t j = 0; j < BlockSize; ++j) {
                data_array[i][j] = 0;
            }
        }
    }

    /*
    __________________________________________________________________
    | Tag (21 bits)       | Index (7 bits)   | Block Offset (4 bits) |
    ------------------------------------------------------------------
    */

    bool is_hit(const uint32_t &address) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address
        
        return (val_array[index] and tag_array[index] == tag);
    }

    bool is_unique(const uint32_t &address) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address
        
        return ((val_array[index] == UC or val_array[index] == UD) and tag_array[index] == tag);
    }
    
    bool access(const uint32_t &address, uint32_t& data) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address
        
        if (is_hit(address)) {
            size_t block_offset = address % BlockSize;
            data = data_array[index][block_offset];
            return true;
        }
        
        // Cache miss
        return false;

        /*
        return mem.read_memory(address, data);
        */
    }

    void update(const uint32_t &address, const uint32_t& new_data) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address

        if (!is_unique(address)) {
            chi_read_unique(address);
        }
        
        // // Update the cache line
        // tag_array[index] = tag;
        // val_array[index] = true;
        // memcpy(data_array[index], new_data, BlockSize * sizeof(uint32_t));
        
        /*
        mem.write_memory(address, new_data);
        */
    }
};

#endif