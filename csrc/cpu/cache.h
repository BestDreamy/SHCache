#include "../include/utils.h"

// Direct-Map Local Cache
template <size_t NumSets = 128, size_t BlockSize = 4>
struct Cache {
    uint32_t tag_array[NumSets];
    uint32_t data_array[NumSets][BlockSize];
    uint32_t val_array[NumSets];

    Cache() {
        for (size_t i = 0; i < NumSets; ++i) {
            tag_array[i] = 0;
            val_array[i] = 0;
            for (size_t j = 0; j < BlockSize; ++j) {
                tag_array[i][j] = 0;
            }
        }
    }

    /*
    | Tag (21 bits)       | Index (7 bits)   | Block Offset (4 bits) |
    */
    bool access(uint32_t address, uint32_t& data) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address

        if (val_array[index] && tag_array[index] == tag) {
            // Cache hit
            size_t block_offset = address % BlockSize;
            data = data_array[index][block_offset];
            return true;
        }

        // Cache miss
        return false;
    }

    void update(uint32_t address, const uint32_t* new_data) {
        size_t index = (address / BlockSize) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * BlockSize); // Extract tag from address

        // Update the cache line
        tag_array[index] = tag;
        val_array[index] = true;
        memcpy(data_array[index], new_data, BlockSize * sizeof(uint32_t));
    }
};