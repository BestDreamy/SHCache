#pragma once
#include <cstdint>
#include "../mem.h"
#include "../include/utils.h"
#include "../cache.h"
#include "../chi/flit/req_flit.h"
#include "../chi/flit/dat_flit.h"
#include "../chi/transaction/req_flow.h"
#include "../chi/transaction/rsp_flow.h"
#include "flit/auto_flit.h"
#include "sf.h"

// system-level cache handle the memory requests from all CPUs
template <size_t numSet = 128, size_t BlockSize = 4>
struct SnoopFilter {

    static constexpr size_t numBlock = 1 << BlockSize;
    paddr_t tag_array[numSet];
    Cache_State val_array[numSet];
    bool rnfVec[numSet][NUMCORES];
    
    SnoopFilter() {
        for (size_t i = 0; i < numSet; ++i) {
            tag_array[i] = 0;
            val_array[i] = I;
            for (size_t c = 0; c < NUMCORES; ++c) {
                rnfVec[i][c] = false;
            }
        }
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

};
