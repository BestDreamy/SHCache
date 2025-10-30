#pragma once
#include <cstdint>
#include <cassert>
#include "../mem.h"
#include "../include/utils.h"
#include "../cache.h"
#include "../chi/flit/req_flit.h"
#include "../chi/flit/dat_flit.h"
#include "../chi/transaction/req_flow.h"
#include "../chi/transaction/rsp_flow.h"
#include "flit/auto_flit.h"

template <size_t numSet = 128, size_t BlockSize = 4>
struct SystemCache: public Cache<numSet, BlockSize> {

    static constexpr size_t numBlock = 1 << BlockSize;
    paddr_t tag_array[numSet];
    uint8_t data_array[numSet][numBlock];
    Cache_State val_array[numSet];

    SystemCache() {
        Cache();
    }

    void exec_req(reqflit_t req) {
        paddr_t base_addr = req.Addr;
        paddr_t index = this->set_of(base_addr);
        paddr_t tag = this->tag_of(base_addr);

        // For ReadUnique request
        // if (req.Opcode == ReadUnique) {
        //     // Install the cache line
        //     this->tag_array[index] = tag;
        //     this->val_array[index] = UC;

        //     // Initialize data to zero
        //     for (size_t i = 0; i < numBlock; ++i) {
        //         this->data_array[index][i] = 0;
        //     }

        //     // Issue CompData response
        //     chi_issue_CompData_rsp(req, this->data_array[index], UC);
        //     return;
        // }

        // Assert(0, "Only ReadUnique is supported in SLC");
    }

    // Lookup
    // inline bool lookup(uint64_t addr, uint8_t &state_out, uint8_t** data_out) {
    //     size_t set = set_of(addr);
    //     uint64_t tag = tag_of(addr);
    //     for (size_t w = 0; w < WAYS; w++) {
    //         if (tagArray[set][w] == tag && stateArray[set][w] != 0) {
    //             state_out = stateArray[set][w];
    //             *data_out = dataArray[set][w];
    //             return true;
    //         }
    //     }
    //     state_out = 0;
    //     *data_out = nullptr;
    //     return false;
    // }

    // // Install new line (FIFO replacement)
    // inline uint8_t* allocate(uint64_t addr, uint8_t newState) {
    //     size_t set = set_of(addr);
    //     uint8_t victim = fifo_ptr[set];

    //     tagArray[set][victim]   = tag_of(addr);
    //     stateArray[set][victim] = newState;

    //     fifo_ptr[set] = (victim + 1) % WAYS;
    //     return dataArray[set][victim];
    // }

    // // Update state only
    // inline void update_state(uint64_t addr, uint8_t newState) {
    //     size_t set = set_of(addr);
    //     uint64_t tag = tag_of(addr);
    //     for (size_t w = 0; w < WAYS; w++) {
    //         if (tagArray[set][w] == tag) {
    //             stateArray[set][w] = newState;
    //             return;
    //         }
    //     }
    // }
};

extern SystemCache<> slc;
