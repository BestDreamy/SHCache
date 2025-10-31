#pragma once
#include <cstdint>
#include "../mem.h"
#include "../include/utils.h"
#include "../cache.h"
#include "../chi/flit/req_flit.h"
#include "../chi/flit/dat_flit.h"
#include "../chi/transaction/req_flow.h"
#include "../chi/transaction/rsp_flow.h"
#include "../chi/flit/auto_flit.h"
#include "sf.h"

// system-level cache handle the memory requests from all CPUs
template <size_t numSet = 4, size_t BlockSize = 4>
struct SystemCache: public Cache<numSet, BlockSize> {
    SnoopFilter<> sf;
    SystemCache() {
        Cache<>();
    }

    void exec_req(const reqflit_t &req) {
        paddr_t aligned_addr = req.Addr;
        paddr_t index = this->set_of(aligned_addr);
        paddr_t tag = this->tag_of(aligned_addr);

        if (req.Opcode == ReadUnique) {
            sf.stash_req_for_rsp(req);
            mem.chi_read_memory_with_DMT(req);
            return;
        }

        Assert(0, "Only ReadUnique is supported in SLC");
    }

    void exec_rsp(const rspflit_t &rsp) {
        if (rsp.Resp == CompAck_UC) {
            sf.exec_unique_rsp(rsp);
            return;
        }
        return;
    }

    void show_snoop() const {
        sf.show_snoop();
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
