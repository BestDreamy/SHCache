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
#include <stack>

enum Snoop_State {
    Invalid, 
    Share = 1,
    Unique = 2,
    Stash = 3 // Ready to be modified
};

static const char* snoopStateToStr(Snoop_State s) {
    switch (s) {
        case Invalid: return "Invlid";
        case Share: return "Share";
        case Unique: return "Unique";
        default: return "?";
    }
}

// system-level cache handle the memory requests from all CPUs
template <size_t numSet = 4, size_t BlockSize = 4>
struct SnoopFilter {

    static constexpr size_t numBlock = 1 << BlockSize;
    paddr_t tag_array[numSet];
    Snoop_State val_array[numSet];
    bool rnfVec[numSet][NUMCORES];
    std::stack<paddr_t> rsp_stash;
    
    SnoopFilter() {
        for (size_t i = 0; i < numSet; ++i) {
            tag_array[i] = 0;
            val_array[i] = Invalid;
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
        // paddr_t tag = addr / (numSet * numBlock); // Extract index from address
        return aligned_addr;
    }

    void stash_req_for_rsp(const reqflit_t &req) {
        paddr_t aligned_addr = req.Addr;
        paddr_t index = this->set_of(aligned_addr);
        paddr_t tag = this->tag_of(aligned_addr);

        rsp_stash.push(aligned_addr);
    }

    void exec_unique_rsp(const rspflit_t &rsp) {
        Assert(!rsp_stash.empty(), "Rsp stash is empty when exec_unique_rsp");

        paddr_t aligned_addr = rsp_stash.top();
        rsp_stash.pop();
        paddr_t index = this->set_of(aligned_addr);
        paddr_t tag = this->tag_of(aligned_addr);

        this->tag_array[index] = tag;
        this->val_array[index] = Unique;

        rnfVec[index][rsp.SrcID] = true;
    }

    void show_snoop() const {
        std::ostringstream oss;
        oss << "===== Directory Dump =====\n";
        oss << std::hex << std::setfill('0');

        for (size_t set = 0; set < numSet; ++set) {
            oss << "[" << std::setw(3) << std::dec << set << "] "
                << "Tag = 0x" << std::hex << std::setw(sizeof(paddr_t)*2) << tag_array[set];

            oss << "   State = " << snoopStateToStr(val_array[set]);

            oss << "   RNF = [";
            bool first = true;
            for (size_t core = 0; core < NUMCORES; core++) {
                if (rnfVec[set][core]) {
                    if (!first) oss << ", ";
                    oss << core;
                    first = false;
                }
            }
            oss << "]\n";
        }

        devLog("%s", oss.str().c_str());
    }

};
