#ifndef MEM_H
#define MEM_H

#include <map>

struct Memory {
    std::map<uint64_t, uint32_t> mem;

    Memory() {
        mem.clear();
    }

    // Read from memory
    bool read_memory(const uint64_t &addr, uint32_t& data) {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            mem[addr] = 0x44332211;
        }
        data = mem[addr];
        return true;
    }

    // Write to memory
    void write_memory(const uint32_t &addr, const uint32_t &data) {
        mem[addr] = data;
    }
};

extern Memory mem;
#endif