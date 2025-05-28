#ifndef CACHE_H
#define CACHE_H
#include "../include/utils.h"
#include "../mem.h"
#include "../chi/transaction/flow.h"
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
    static constexpr size_t numBlock = 1 << BlockSize;
    uint32_t tag_array[NumSets];
    uint8_t data_array[NumSets][numBlock]; // 4 int per set
    cpuCacheState val_array[NumSets];

    reqflit_t RN_Tracker[config.numCreditsForHNReq[0]];
    bool RN_Tracker_valid[config.numCreditsForHNReq[0]];

    Cache() {
        
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

    int find_first_empty_RN_Tracker() const {
        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) {
            if (RN_Tracker_valid[i] == false) {
                return i;
            }
        }
        return -1;
    }

    int find_RN_Tracker_from_CompData(const datflit_t &data) const {
        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) {
            if (RN_Tracker_valid[i] == false) continue;
            if (RN_Tracker[i].TxnID == data.TxnID && RN_Tracker[i].TgtID == data.HomeNID) {
                return i;
            }
        }
        return -1;
    }

    int RN_Tracker_pop(const datflit_t &data) {
        dbgLog("RN Tracker pop");
        int id = find_RN_Tracker_from_CompData(data);
        Exit(id != -1, "No available RN_Tracker");
        
        RN_Tracker_valid[id] = false;
        return id;
    }

    int RN_Tracker_push(const reqflit_t &req) {
        dbgLog("RN Tracker push");
        int id = find_first_empty_RN_Tracker();
        Exit(id != -1, "No available RN_Tracker");
        
        RN_Tracker[id] = req;
        RN_Tracker_valid[id] = true;
        return id;
    }

    /*
    __________________________________________________________________
    | Tag (21 bits)       | Index (7 bits)   | Block Offset (4 bits) |
    ------------------------------------------------------------------
    */

    bool is_hit(const uint32_t &address) const {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address
        
        return (val_array[index] and tag_array[index] == tag);
    }

    bool is_unique(const uint32_t &address) const {
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

    // If the cache line is not valid
    // issue a ReadUnique request
    void update(
        Vmodule* dut, VerilatedFstC* tfp,
        const int &coreId, const uint32_t &address, const uint32_t& new_data
    ) {
        size_t index = (address / numBlock) % NumSets; // Extract index from address
        uint32_t tag = address / (NumSets * numBlock); // Extract tag from address

        if (!is_unique(address)) {
            reqflit_t req = chi_issue_ReadUnique_req(dut, tfp, coreId, address, BlockSize);
            
            RN_Tracker_push(req);
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
        int id = RN_Tracker_pop(data);
        reqflit_t req = RN_Tracker[id];

        uint32_t addr = req.Addr;
        size_t index = (addr / numBlock) % NumSets; // Extract index from address
        uint32_t tag = addr / (NumSets * numBlock); // Extract tag from address

        tag_array[index] = tag;
        val_array[index] = static_cast<cpuCacheState>(data.Resp);

        for (int word = 0; word < data.Data.size() / 32; word ++) {
            uint32_t BE = data.BE;
            uint8_t be_nibble = (BE >> (4 * word)) & 0xF;

            for (int byte_in_word = 0; byte_in_word < 4; byte_in_word ++) {
                uint8_t src_byte = 0;
                for (int bit = 0; bit < 8; bit++) {
                    int bit_index = word * 32 + byte_in_word * 8 + bit;
                    if (data.Data.test(bit_index))
                        src_byte |= (1 << bit);
                }

                if (be_nibble & (1 << byte_in_word))
                    data_array[index][word * 4 + byte_in_word] = src_byte;
                else
                    data_array[index][word * 4 + byte_in_word] = 0;
            }
        }

    }

    void show_cache() const {
        std::cout << "Cache Information:" << std::endl;
        std::cout << "Number of Sets: " << NumSets << std::endl;
        std::cout << "Block Size (numBlock): " << numBlock << " entries per set." << std::endl;
        for (size_t set = 0; set < NumSets; set++) {
            std::cout << "Set " << std::setw(3) << set << ": ";
            std::cout << "Tag = 0x" << std::hex << tag_array[set] << std::dec << ", ";
            std::cout << "State = " << val_array[set] << ", Data = [";
            for (size_t blk = 0; blk < numBlock; blk++) {
                std::cout << static_cast<int>(data_array[set][blk]);
                if (blk != numBlock - 1)
                    std::cout << ", ";
            }
            std::cout << "]" << std::endl;
        }
    }

    void show_RN_Tracker() const {
        std::cout << "RN_Tracker Information:" << std::endl;
        for (int i = 0; i < config.numCreditsForHNReq[0]; i ++) {
            if (RN_Tracker_valid[i]) {
                std::cout << "ID: " << i << ", TxnID: " << static_cast<unsigned>(RN_Tracker[i].TxnID)
                          << ", SrcID: " << static_cast<unsigned>(RN_Tracker[i].SrcID) << std::endl;
            }
        }
        std::cout << "----------------------------------------" << std::endl;
    }
};

#endif