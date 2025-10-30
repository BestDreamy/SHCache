#ifndef L1CACHE_H
#define L1CACHE_H
#include "../include/utils.h"
#include "../include/dbg.h"
#include "../mem.h"
#include "../include/autoconfig.h"
#include <cstddef>
#include <cstring>
#include "../chi/flit/req_flit.h"
#include "../chi/flit/dat_flit.h"
#include "../chi/transaction/req_flow.h"
#include "../chi/transaction/rsp_flow.h"
#include "../cache.h"

// enum CPU_Cache_State {
//     I = 0,
//     SC = 1,
//     UC = 2,
//     UD = 6,
//     SD = 7,

//     Save = 3
// };

// Direct-Map Local Cache
template <size_t numSet = 128, size_t BlockSize = 4>
struct L1Cache: public Cache<numSet, BlockSize> {

    reqflit_t RN_Tracker[config.numCreditsForHNReq[0]];
    bool RN_Tracker_valid[config.numCreditsForHNReq[0]];

    L1Cache() {
        
        Cache();

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
        devLog("RN Req Tracker pop");
        int id = find_RN_Tracker_from_CompData(data);
        Exit(id != -1, "No available RN_Tracker");
        
        RN_Tracker_valid[id] = false;
        return id;
    }

    int RN_Tracker_push(const reqflit_t &req) {
        devLog("RN Req Tracker push");
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
    
    bool access(
        const int &coreId, const paddr_t &addr, uint32_t& data
    ) {
        paddr_t aligned_addr = this->aligned_of(addr);

        paddr_t index = this->set_of(addr);
        paddr_t tag = this->tag_of(addr);

        if (this->is_hit(aligned_addr)) {
            memcpy(&data, this->data_array[index], sizeof(uint32_t));
            return true;
        }
        
        // Same as cache.update()
        if (!this->is_unique(aligned_addr)) {
            reqflit_t req = chi_issue_ReadUnique_req(coreId, aligned_addr, BlockSize);
            
            RN_Tracker_push(req);

            return false;
        }
        Assert(0, "Other states are not supported yet");
    }

    // If the cache line is not valid
    // issue a ReadUnique request
    bool update(
        const int &coreId, const paddr_t &addr, const uint32_t& new_data
    ) {
        if (this->is_hit(addr)) {
            this->update_cacheline_by_word(addr, new_data, Save);
            return true;
        }
        
        paddr_t aligned_addr = this->align_of(addr);
        if (!this->is_unique(aligned_addr)) {
            reqflit_t req = chi_issue_ReadUnique_req(coreId, aligned_addr, BlockSize);
            
            RN_Tracker_push(req);

            return false;
        }
        Assert(0, "Other states are not supported yet");
    }

    void update(const datflit_t &data) {
        int id = RN_Tracker_pop(data);    // Pop tracker entry
        const reqflit_t &req = RN_Tracker[id];
        paddr_t base_addr = req.Addr;

        const size_t word_count = data.Data.size() / 32;

        for (size_t word = 0; word < word_count; ++word) {
            uint8_t be_mask = (data.BE >> (4 * word)) & 0xF;
            if (be_mask == 0) continue;

            uint32_t word_value = 0;
            for (int bit = 0; bit < 32; ++bit) {
                if (data.Data.test(word * 32 + bit)) {
                    word_value |= (1u << bit);
                }
            }

            for (int byte = 0; byte < 4; ++byte) {
                if (be_mask & (1 << byte)) {
                    uint8_t byte_src = (word_value >> (byte * 8)) & 0xFF;
                    this->update_cacheline_by_byte(
                        base_addr + word * 4 + byte,
                        byte_src,
                        (data.Resp == CompData_UC) ? UC : UD
                    );
                }
            }
        }

        chi_issue_CompAck_rsp(data);
    }


    void show_cache() const {
        logFile << "Number of Sets: " << numSet << std::endl;
        logFile << "Block Size (numBlock): " << this->numBlock << " bytes per set." << std::endl;
        for (size_t set = 0; set < numSet; set++) {
            if (this->val_array[set] == I) continue; // Skip invalid sets
            logFile << "Set " << std::setw(3) << set << ": ";
            logFile << "Tag = 0x" << std::hex << this->tag_array[set] << std::dec << ", ";
            logFile << "State = " << this->val_array[set] << ", Data = [";
            for (size_t blk = 0; blk < this->numBlock; blk++) {
                logFile << "0x" << std::hex << static_cast<int>(this->data_array[set][blk]) << std::dec;
                if (blk != this->numBlock - 1)
                    logFile << ", ";
            }
            logFile << "]" << std::endl;
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
