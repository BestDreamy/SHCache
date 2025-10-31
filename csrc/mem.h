#ifndef MEM_H
#define MEM_H

#include <map>
#include "chi/flit/auto_flit.h"
#include "chi/flit/req_flit.h"
#include "chi/flit/dat_flit.h"
#include "chi/flit/rsp_flit.h"
#include "chi/rnf_utils.h"

typedef uint32_t paddr_t;

struct Memory {
    std::map<paddr_t, uint32_t> mem;

    Memory() {
        mem.clear();
    }

    // Read from memory
    bool read_memory(const paddr_t &addr, uint32_t& data) {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            mem[addr] = 0x44332211; // Test Mem Data
            // mem[addr] = 0;
        }
        data = mem[addr];
        return true;
    }

    // Write to memory
    void write_memory(const paddr_t &addr, const uint32_t &data) {
        mem[addr] = data;
    }

    // AMBA5 CHI

    void chi_read_memory_with_DMT(const reqflit_t &req) {
        datflit_t dat = createCompData_UC(req);
        RN_dat_channel[req.StashNID_ReturnNID].push(dat);
    }
};

extern Memory mem;
#endif