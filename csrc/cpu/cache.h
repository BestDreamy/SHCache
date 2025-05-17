#ifndef CACHE_H
#define CACHE_H
#include "../include/utils.h"
#include "../mem.h"
#include "../chi/transaction.h"
#include "../include/autoconfig.h"

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
    #define numBlock  1 << BlockSize
    uint32_t tag_array[NumSets];
    uint8_t data_array[NumSets][numBlock]; // 4 int per set
    cpuCacheState val_array[NumSets];
    bool access_finished;

    reqflit_t RN_Tracker[config.numCreditsForHNReq[0]];
    bool RN_Tracker_valid[config.numCreditsForHNReq[0]];

    Cache() {
        access_finished = false;
        
        for (size_t i = 0; i < NumSets; ++i) {
            tag_array[i] = 0;
            val_array[i] = I;
            for (size_t j = 0; j < numBlock; ++j) {
                data_array[i][j] = 0;
            }
        }

        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) 
            RN_Tracker_valid[i] = false;
    }

    int find_first_RN_Tracker() {
        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) {
            if (RN_Tracker_valid[i] == false) {
                return i;
            }
        }
        return -1;
    }

    int find_RN_Tracker_from_CompData(const datflit_t &data) {
        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) {
            if (RN_Tracker[i].TxnID == data.TxnID) {
                return i;
            }
        }
        return -1;
    }

    /*
    __________________________________________________________________
    | Tag (21 bits)       | Index (7 bits)   | Block Offset (4 bits) |
    ------------------------------------------------------------------
    */

    bool is_hit(const uint32_t &address) {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address
        
        return (val_array[index] and tag_array[index] == tag);
    }

    bool is_unique(const uint32_t &address) {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address
        
        return ((val_array[index] == UC or val_array[index] == UD) and tag_array[index] == tag);
    }
    
    bool access(const uint32_t &address, uint32_t& data) {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address
        
        if (is_hit(address)) {
            size_t block_offset = address % numBlock;
            memcpy(&data, &data_array[index][block_offset], sizeof(uint32_t));
            return true;
        }
        
        // Cache miss
        return false;

        /*
        return mem.read_memory(address, data);
        */
    }

    void update(
        Vmodule* dut, VerilatedFstC* tfp,
        const int &coreId, const uint32_t &address, const uint32_t& new_data
    ) {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address

        if (!is_unique(address)) {
            reqflit_t req = chi_issue_ReadUnique_req(dut, tfp, coreId, address, BlockSize);

            int id = find_first_RN_Tracker();
            Assert(id != -1, "No available RN_Tracker");

            RN_Tracker[id] = req;
            RN_Tracker_valid[id] = true;
        }
        
        // // Update the cache line
        // tag_array[index] = tag;
        // val_array[index] = true;
        // memcpy(data_array[index], new_data, numBlock * sizeof(uint32_t));
        
        /*
        mem.write_memory(address, new_data);
        */
    }

    void update(const datflit_t &data) {
        int id = find_RN_Tracker_from_CompData(data);
        size_t index = (data.Addr / numBlock) % NumSets; // Extract index from address
        uint32_t tag = data.Addr / (NumSets * numBlock); // Extract tag from address

        tag_array[index] = tag;
        val_array[index] = static_cast<cpuCacheState>(data.Resp);
        memcpy(data_array[index], data.Data, numBlock * sizeof(uint32_t));
    }
};

#endif